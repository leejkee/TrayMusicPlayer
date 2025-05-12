//
// Created by cww on 25-4-14.
//
#include "../core/databasemanager/DatabaseManager.h"
#include "../core/common/Song.h"

#include <QTest>
#include <QSqlError>
#include <QFile>
#include <QTemporaryDir>

using DBM = Tray::Core::DatabaseManager;
using Music = Tray::Core::Song;

class TestDatabaseManager : public QObject {
    Q_OBJECT

public:
    TestDatabaseManager();
    ~TestDatabaseManager() override;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testOpenClose();
    void testCreateTable_data();
    void testCreateTable();
    void testInsertReadDeleteSong();
    void testDeleteSongWithTitle_Nonexistent();
    void testDeleteTable();
    void testInvalidTableNameOps();

private:
    QTemporaryDir *m_tempDir = nullptr;
    QString m_dbPath;
    DBM *m_dbManager = nullptr;
    const QString m_testTableName = "TestSongs";
    const QString m_connectionName = "TestDBConnection";

    // Dummy data constants
    const QString MUSIC_PATH = "/path/to/dummy/music.mp3";
    const QString DUMMY_TITLE_1 = "Test Song 1";
    const QString DUMMY_PATH_1 = MUSIC_PATH + "1";
    const QString DUMMY_NAME_1 = "Song Name 1";
    const QString DUMMY_ARTIST_1 = "Artist 1";
    const int DUMMY_DURATION_1 = 180;

    const QString DUMMY_TITLE_2 = "Test Song 2";
    const QString DUMMY_PATH_2 = MUSIC_PATH + "2";
    const QString DUMMY_NAME_2 = "Song Name 2";
    const QString DUMMY_ARTIST_2 = "Artist 2";
    const int DUMMY_DURATION_2 = 240;
};

TestDatabaseManager::TestDatabaseManager() = default;

TestDatabaseManager::~TestDatabaseManager() = default;

void TestDatabaseManager::initTestCase() {
    m_tempDir = new QTemporaryDir();
    QVERIFY2(m_tempDir->isValid(), "Failed to create temporary directory");
    m_dbPath = m_tempDir->filePath("test_tray_db.sqlite");

    // Ensure no connection with this name exists before creating a new one
    if (QSqlDatabase::contains(m_connectionName)) {
         QSqlDatabase::removeDatabase(m_connectionName);
    }

    m_dbManager = new DBM(m_dbPath, m_connectionName, this);
    QVERIFY2(m_dbManager->isOpen(), "Database did not open successfully");
}

void TestDatabaseManager::cleanupTestCase() {
    delete m_dbManager; // This should close and remove the database connection
    m_dbManager = nullptr;

    if (m_tempDir) {
        m_tempDir->remove(); // Clean up the temporary directory and database file
        delete m_tempDir;
        m_tempDir = nullptr;
    }

    // Ensure the connection is truly removed
     if (QSqlDatabase::contains(m_connectionName)) {
         QSqlDatabase::removeDatabase(m_connectionName);
     }
}

void TestDatabaseManager::testOpenClose() {
    // initTestCase already verified open
    QVERIFY(m_dbManager->isOpen());

    // Test closing - relies on destructor, but we can test if the connection is still valid
    // This is implicitly tested by subsequent operations, but a simple check after deletion is hard here.
    // The init/cleanup cycle tests the open/close process implicitly.
}

void TestDatabaseManager::testCreateTable_data() {
    QTest::addColumn<QString>("tableName");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("valid table name") << m_testTableName << true;
    QTest::newRow("valid table name with underscore") << "my_songs_table" << true;
    QTest::newRow("valid table name with number") << "songs123" << true;
    QTest::newRow("invalid table name - starts with number") << "123songs" << false;
    QTest::newRow("invalid table name - contains space") << "my songs" << false;
    QTest::newRow("invalid table name - contains special char") << "songs-table" << false;
    QTest::newRow("invalid table name - empty") << "" << false;
}

void TestDatabaseManager::testCreateTable() {
    QFETCH(QString, tableName);
    QFETCH(bool, expectedResult);

    bool result = m_dbManager->createTable(tableName);
    QCOMPARE(result, expectedResult);

    // Verify the table exists if creation was expected to succeed
    if (expectedResult) {
        QSqlQuery query(m_dbManager->m_databaseConnection); // Accessing private member for verification
        query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=?");
        query.addBindValue(tableName);
        QVERIFY(query.exec());
        QVERIFY(query.next()); // Should find the table
        QCOMPARE(query.value(0).toString(), tableName);
    }
}


void TestDatabaseManager::testInsertReadDeleteSong() {
    // Ensure the test table exists
    QVERIFY(m_dbManager->createTable(m_testTableName));

    // Create dummy songs
    Music song1;
    song1.m_title = DUMMY_TITLE_1;
    song1.m_path = DUMMY_PATH_1;
    song1.m_name = DUMMY_NAME_1;
    song1.m_artist = DUMMY_ARTIST_1;
    song1.m_duration = DUMMY_DURATION_1;

    Music song2;
    song2.m_title = DUMMY_TITLE_2;
    song2.m_path = DUMMY_PATH_2;
    song2.m_name = DUMMY_NAME_2;
    song2.m_artist = DUMMY_ARTIST_2;
    song2.m_duration = DUMMY_DURATION_2;

    // Test Insert
    QVERIFY2(m_dbManager->insertSong(m_testTableName, song1), "Failed to insert song 1");
    QVERIFY2(m_dbManager->insertSong(m_testTableName, song2), "Failed to insert song 2");

    // Test Read All
    QVector<Music> songs = m_dbManager->readAllSongsFromTable(m_testTableName);
    QCOMPARE(songs.size(), 2);

    // Verify inserted data (order might not be guaranteed without ORDER BY, so check for presence)
    bool foundSong1 = false;
    bool foundSong2 = false;
    for (const auto& song : songs) {
        if (song.m_title == DUMMY_TITLE_1 &&
            song.m_path == DUMMY_PATH_1 &&
            song.m_name == DUMMY_NAME_1 &&
            song.m_artist == DUMMY_ARTIST_1 &&
            song.m_duration == DUMMY_DURATION_1) {
            foundSong1 = true;
        }
         if (song.m_title == DUMMY_TITLE_2 &&
            song.m_path == DUMMY_PATH_2 &&
            song.m_name == DUMMY_NAME_2 &&
            song.m_artist == DUMMY_ARTIST_2 &&
            song.m_duration == DUMMY_DURATION_2) {
            foundSong2 = true;
        }
    }
    QVERIFY2(foundSong1, "Song 1 not found in read results");
    QVERIFY2(foundSong2, "Song 2 not found in read results");

    // Test Delete Song 1
    QVERIFY2(m_dbManager->deleteSongWithTitle(m_testTableName, DUMMY_TITLE_1), "Failed to delete song 1");

    // Verify song 1 is deleted
    songs = m_dbManager->readAllSongsFromTable(m_testTableName);
    QCOMPARE(songs.size(), 1);
    QCOMPARE(songs[0].m_title, DUMMY_TITLE_2);

    // Test Delete Song 2
    QVERIFY2(m_dbManager->deleteSongWithTitle(m_testTableName, DUMMY_TITLE_2), "Failed to delete song 2");

    // Verify song 2 is deleted
    songs = m_dbManager->readAllSongsFromTable(m_testTableName);
    QCOMPARE(songs.size(), 0);
}

void TestDatabaseManager::testDeleteSongWithTitle_Nonexistent() {
     // Ensure the test table exists
    QVERIFY(m_dbManager->createTable(m_testTableName));

    // Try deleting a song that doesn't exist
    QVERIFY(m_dbManager->deleteSongWithTitle(m_testTableName, "Nonexistent Song Title"));

    // Verify the table is still empty
    QVector<Music> songs = m_dbManager->readAllSongsFromTable(m_testTableName);
    QCOMPARE(songs.size(), 0);
}

void TestDatabaseManager::testDeleteTable() {
     // Ensure the test table exists
    QVERIFY(m_dbManager->createTable(m_testTableName));

    // Verify table exists before deletion
    QSqlQuery queryBefore(m_dbManager->m_databaseConnection); // Accessing private member
    queryBefore.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=?");
    queryBefore.addBindValue(m_testTableName);
    QVERIFY(queryBefore.exec());
    QVERIFY(queryBefore.next());

    // Test Delete Table
    QVERIFY2(m_dbManager->deleteTable(m_testTableName), "Failed to delete table");

    // Verify table no longer exists
    QSqlQuery queryAfter(m_dbManager->m_databaseConnection); // Accessing private member
    queryAfter.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name=?");
    queryAfter.addBindValue(m_testTableName);
    QVERIFY(queryAfter.exec());
    QVERIFY(!queryAfter.next()); // Should not find the table
}

void TestDatabaseManager::testInvalidTableNameOps() {
    const QString invalidTableName = "invalid-table!";

    // Test operations with invalid table name
    QVERIFY(!m_dbManager->createTable(invalidTableName));
    QVERIFY(!m_dbManager->deleteTable(invalidTableName));
    QVERIFY(!m_dbManager->insertSong(invalidTableName, Music())); // Inserting a dummy song
    QCOMPARE(m_dbManager->readAllSongsFromTable(invalidTableName).size(), 0);
    QVERIFY(!m_dbManager->deleteSongWithTitle(invalidTableName, "Any Title"));
}

// Add the deleteTable implementation from your DatabaseManager.cpp here if you didn't include the whole .cpp
// For the provided header, this function is missing. Assuming it exists in the actual .cpp.
// If not, you'll need to implement it:
/*
bool Tray::Core::DatabaseManager::deleteTable(const QString &tableName) {
    if (!isValidTableName(tableName)) {
        Log.log(Log::QLogger::LogLevel::Error, "Invalid table name for deletion: " + tableName);
        return false;
    }

    QSqlQuery query(m_databaseConnection);
    const QString sql = QString("DROP TABLE IF EXISTS %1").arg(tableName);

    if (!query.exec(sql)) {
        Log.log(Log::QLogger::LogLevel::Error,
                "Delete table failed: " + tableName + ": " + query.lastError().text());
        return false;
    }
    return true;
}
*/


QTEST_MAIN(TestDatabaseManager)
#include "TestDB.moc" // This line is required for QTestQTEST_MAIN(TestDatabaseManager)
#include "TestDB.moc"
