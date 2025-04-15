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

        [[nodiscard]] QList<Song> findList(const QString &listName) const;

    private:
        QHash<QString, QList<Song> > m_listCache;
        Logger_QT Log;

        static QList<Song> loadLocalMusic(const QStringList &localDir);
    };

    class List {
    public:
        List() = default;

        List(const QString &name, const QVector<Song> &list, QObject *parent = nullptr);

    private:
        QString m_listName;
        QVector<Song> m_list;
    };
}
