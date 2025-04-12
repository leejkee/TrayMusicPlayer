//
// Created by cww on 25-4-7.
//
#include <ListCache.h>
#include <CoreConstants.h>
#include <QDirIterator>


namespace Core::Service {
    QList<Song> ListCache::loadLocalMusic(const QStringList &localDir) {
        QList<Song> localList;
        for (const auto &filePath: localDir) {
            QDirIterator it(filePath, MUSIC_FILTERS, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                Song song(it.filePath());
                localList.append(song);
            }
        }
        return localList;
    }

    ListCache::ListCache(const QStringList &localDir, QObject *parent) : QObject(parent) {
        setObjectName(QStringLiteral("ListCache"));
        Log = Logger_QT(this->objectName());
        m_listCache[LOCAL_LIST_KEY] = loadLocalMusic(localDir);
        Log.log(Service::Logger_QT::LogLevel::Info, "ListCache: Local list has been initialized.");
    }

    QList<Song> ListCache::findList(const QString &listName) const {
        if (m_listCache.contains(listName)) {
            return m_listCache.value(listName);
        }
        Log.log(Logger_QT::LogLevel::Warning, "No such PlayList called \"" + listName + "\"");
        return {};
    }

}
