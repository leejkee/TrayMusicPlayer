//
// Created by cww on 25-4-7.
//
#pragma once

#include <Song.h>
#include <Logger_qt.h>
#include <QObject>
#include <QHash>


namespace Core::Service {
    class ListCache final : public QObject {
        Q_OBJECT

    public:
        ListCache() = default;

        explicit ListCache(const QStringList &, QObject *parent = nullptr);

        /// Retrieves the cached list of songs by list name.
        /// @param listName The key/name of the song list.
        /// @return The list of songs associated with the given name,
        ///         or an empty list if not found.
        [[nodiscard]] QVector<Song> findList(const QString &listName) const;

        /// Adds or appends a user-defined playlist to the cache.
        /// @param key The name/key of the user-defined playlist.
        /// @param list The list of songs to store.
        ///        If the key already exists, songs will be appended to the existing list.
        void loadUserList(const QString &key, const QVector<Song> &list);

        /// Loads local music files from the given directories into the cache.
        /// @param localDir A list of directories to scan for music files.
        ///        All matching files will be added under the LOCAL_LIST_KEY.
        void loadLocalMusic(const QStringList &localDir);

    private:
        QHash<QString, QVector<Song> > m_listCache;
        Logger_QT Log;
    };
}
