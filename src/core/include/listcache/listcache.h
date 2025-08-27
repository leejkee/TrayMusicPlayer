//
// Created by cww on 25-4-7.
//
#pragma once

#include <musicmetadata.h>
#include <QObject>
#include <QHash>


namespace Tray::Core
{
class ListCache final : public QObject
{
    Q_OBJECT

public:
    explicit ListCache(QObject* parent = nullptr);

    /// This constructor will call @loadUserPlaylists() and @loadLocalPlaylist()
    ListCache(const QStringList& localDir
              , const QStringList& userListKeys
              , QObject* parent = nullptr);

public Q_SLOTS:
    void init(const QStringList& localDir, const QStringList& userListKeys);

    /// Retrieves the cached list of songs by list name.
    /// @param listName The key/name of the song list.
    /// @return The list of songs associated with the given name,
    ///         or an empty list if not found.
    [[nodiscard]] QList<Tray::Core::MusicMetaData> findList(const QString& listName) const;

    /// Adds or appends a user-defined playlist to the cache.
    /// @param userListKeys The keys of user playlist
    /// This function will read playlists from Database
    void initUserPlaylists(const QStringList& userListKeys);

    /// Retrieves the title list of songs by list name
    /// @param name The name of the song list
    /// @return The title list of songs associated with the given name, using findList()
    [[nodiscard]] QStringList getMusicTitleList(const QString& name) const;

    /// Loads local music files from the given directories into the cache.
    /// @param localDir A list of directories to scan for music files.
    ///        All matching files will be added under the LOCAL_LIST_KEY.
    void initLocalPlaylist(const QStringList& localDir);

    /// create an empty vector map to key
    /// @param key user list key
    void newUserPlaylist(const QString& key);

    void deleteUserPlaylist(const QString& key);

    /// copy a song from source to destination with an index
    /// @param sourceKey the name of list
    /// @param destinationKey
    /// @param index
    bool copyMusicFromListToList(const QString& sourceKey
                                 , const QString& destinationKey
                                 , int index);

    void removeSongFromListByIndex(const QString& key, int index);

    void removeSongFromListByTitle(const QString& key
                                   , const QString& songTitle);

    void respondMusicTitleList(const QString& key);

    void handleSwitchPlaylistAndPlayIndex(const QString& key, int index);

Q_SIGNALS:
    // emitted when cache modified
    void signalNotifyPlayListCacheModified(const QString& key
                                           , const QList<Tray::Core::MusicMetaData>& list);

    void signalNotifyUiCacheModified(const QString& key
                                     , const QStringList& list);

    void signalSendUiCurrentTitleList(const QString& key
                                      , const QStringList& titleList);

    void signalUserPlaylistCreated(const QString& listName);

    void signalUserPlaylistDeleted(const QString& key);

    void signalInitCompleted();

    void signalRespondPlayListSwitchAndPlayIndex(
        const QString& key
        , const QList<Tray::Core::MusicMetaData>& list
        , int index);

private:
    static inline const QStringList MUSIC_FILTERS = {
        QStringLiteral("*.mp3")
        , QStringLiteral("*.flac")
    };

    /// @brief Sets the playlist associated with a given key.
    /// @param key The unique key identifying the playlist to be updated.
    /// @param list The vector of Song objects that will replace the current playlist content.
    void setList(const QString& key, const QList<MusicMetaData>& list);

    QHash<QString, QList<MusicMetaData>> m_listCache;
};
}
