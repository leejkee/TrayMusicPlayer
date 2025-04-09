//
// Created by cww on 25-4-9.
//
#pragma once

#include <QAbstractListModel>

namespace UI::Panel {
    class DataModel final : public QAbstractListModel {
    public:
        explicit DataModel(QObject *parent = nullptr) : QAbstractListModel(parent) {
        }

        struct SongInfo {
            QString name;
            QString artist;
        };

        void setSongs(const QStringList &list);

    protected:
        [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    private:
        QList<SongInfo> m_list{};

        /// @param str should be the @m_fullName
        /// @return the artist name
        static QString convertToArtist(const QString &str);

        /// @param str should be the @m_fullName
        /// @return the song name with no suffix "- artist"
        static QString convertToName(const QString &str);
    };
}
