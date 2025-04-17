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

        [[nodiscard]] QVector<Song> findList(const QString &listName) const;

    private:
        QHash<QString, QVector<Song> > m_listCache;
        Logger_QT Log;

        static QVector<Song> loadLocalMusic(const QStringList &localDir);
    };

}
