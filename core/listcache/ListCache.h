//
// Created by cww on 25-4-7.
//
#pragma once

#include "Song.h"
#include <QLogger.h>
#include <QObject>
#include <QHash>


namespace Tray::Core {
    class ListCache final : public QObject {
        Q_OBJECT

    public:
        ListCache() = default;



        explicit ListCache(QObject *parent = nullptr);

        /// This constructor will call @loadUserPlaylists() and @loadLocalPlaylist()
        ListCache(const QStringList &localDir, const QStringList &userListKeys, QObject *parent = nullptr);


        /// Retrieves the cached list of songs by list name.
        /// @param listName The key/name of the song list.
        /// @return The list of songs associated with the given name,
        ///         or an empty list if not found.
        [[nodiscard]] QVector<Song> findList(const QString &listName) const;


        /// Adds or appends a user-defined playlist to the cache.
        /// @param userListKeys The keys of user playlist
        /// This function will read playlists from Database
        void initUserPlaylists(const QStringList &userListKeys);

        /// Retrieves the title list of songs by list name
        /// @param name The name of the song list
        /// @return The title list of songs associated with the given name, using findList()
        QStringList getMusicTitleList(const QString &name) const;

        /// Loads local music files from the given directories into the cache.
        /// @param localDir A list of directories to scan for music files.
        ///        All matching files will be added under the LOCAL_LIST_KEY.
        void initLocalPlaylist(const QStringList &localDir);

        /// create an empty vector map to key
        /// @param key user list key
        void newUserPlaylist(const QString &key);

        void insertMusicToList(const QString &key, const Song &song);

    Q_SIGNALS:
        void signalMusicInserted(const QString &listName, const Song &song);

    private:
        static inline const QStringList MUSIC_FILTERS = {
            QStringLiteral("*.mp3"),
            QStringLiteral("*.flac")
        };

        QHash<QString, QVector<Song> > m_listCache;
        Log::QLogger Log;
    };
}
