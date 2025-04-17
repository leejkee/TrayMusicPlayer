//
// Created by cww on 25-4-14.
//
#include <QTest>
#include <DatabaseManager.h> // assuming your header is named this

using DBM = Core::Service::DatabaseManager;
using Music = Core::Service::Song;

class TestDatabaseManager : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();

    void cleanupTestCase();

    void testCreateTable();

    void testInsertSong();

    void testLoadAllSongs();

    void testDeleteSong();

private:
    QString dbPath;
    QString connectionName;
    QString tableName;
    DBM *dbManager;
    Music testSong;
};

void TestDatabaseManager::initTestCase() {
    dbPath = "C:/Users/cww/Documents/Workspace/TrayMusicPlayer/core/music.db";
    connectionName = "TestConnection";
    tableName = "QTestTable";

    // Windows 路径转为 Qt 可用路径
    const QString absPath = QDir::fromNativeSeparators("C:\\Users\\cww\\Music\\lostgrace\\知我-国风堂,哦漏.mp3");

    // 构造测试用 Song
    testSong = Music(absPath); // Song(absPath) 构造函数会自动填充其他字段

    dbManager = new DBM(dbPath, connectionName);
    QVERIFY(dbManager->isOpen());
}

void TestDatabaseManager::cleanupTestCase() {
    QVERIFY(dbManager->deleteTable(tableName));
    delete dbManager;
}

void TestDatabaseManager::testCreateTable() {
    QVERIFY(dbManager->createTable(tableName));
}

void TestDatabaseManager::testInsertSong() {
    QVERIFY(dbManager->insertSong(tableName, testSong));
}

void TestDatabaseManager::testLoadAllSongs() {
    QVector<Music> songs = dbManager->loadAllSongs(tableName);
    QVERIFY(!songs.isEmpty());

    bool found = false;
    for (const Music &s: songs) {
        if (s.m_path == testSong.m_path) {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestDatabaseManager::testDeleteSong() {
    QVERIFY(dbManager->deleteSong(tableName, testSong.m_path));
}

QTEST_MAIN(TestDatabaseManager)
#include "TestDB.moc"
