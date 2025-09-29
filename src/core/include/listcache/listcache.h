//
// Created by cww on 25-4-7.
//
#pragma once

#include <musicmetadata.h>
#include <QObject>
#include <QHash>
#include <functional>

namespace Tray::Core
{
class ListCache final : public QObject
{
    Q_OBJECT

public:
    explicit ListCache(QObject* parent = nullptr);

    /// This constructor will call @loadUserPlaylists() and @loadLocalPlaylist()
    ListCache(const QStringList& localPaths
              , const QStringList& userListKeys
              , QObject* parent = nullptr);

    /// Retrieves the cached list of songs by list name.
    /// @param listName The key/name of the song list.
    /// @return The list of songs associated with the given name,
    ///         or an empty list if not found.
    [[nodiscard]] std::shared_ptr<QList<MusicMetaData>> getPlaylist(
        const QString& listName) const;

public Q_SLOTS:
    /// Adds or appends a user-defined playlist to the cache.
    /// @param userListKeys The keys of user playlist
    /// This function will read playlists from Database
    void initUserPlaylists(const QStringList& userListKeys);

    /// Retrieves the title list of songs by list name
    /// @param key The name of the song list
    /// @return The title list of songs associated with the given name, using findList()
    [[nodiscard]] QStringList getMusicTitleList(const QString& key) const;

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

Q_SIGNALS:
    // emitted when cache modified
    void signalPlaylistCacheModified(const QString& key
                                     , std::shared_ptr<QList<MusicMetaData>>
                                     list);

    void signalNotifyUiCacheModified(const QString& key
                                     , const QStringList& list);

    void signalSendUiCurrentTitleList(const QString& key
                                      , const QStringList& titleList);

    void signalUserPlaylistCreated(const QString& listName);

    void signalUserPlaylistDeleted(const QString& key);

private:
    static inline const QStringList MUSIC_FILTERS = {
        QStringLiteral("*.mp3")
        , QStringLiteral("*.flac")
    };

    QHash<QString, std::shared_ptr<QList<MusicMetaData>>> m_listCache;

    void modifyCache(const QString& key
                     , std::function<void(QList<MusicMetaData>&)> f);
};
}
