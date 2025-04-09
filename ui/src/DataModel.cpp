//
// Created by cww on 25-4-10.
//
#include <panel/DataModel.h>


namespace UI::Panel {
    void DataModel::setSongs(const QStringList &list) {
        beginResetModel();
        m_list.clear();
        for (const auto &song: list) {
            SongInfo info;
            info.name = convertToName(song);
            info.artist = convertToArtist(song);
            m_list.append(info);
        }
        endResetModel();
    }

    int DataModel::rowCount(const QModelIndex &parent) const {
        return parent.isValid() ? 0 : static_cast<int>(m_list.size());
    }

    QVariant DataModel::data(const QModelIndex &index, const int role) const {
        if (!index.isValid() || index.row() >= m_list.size()) return {};
        const auto &[name, artist] = m_list[index.row()];
        switch (role) {
            case Qt::DisplayRole: return name;
            case Qt::UserRole: return artist;
            default: return {};
        }
    }

    QString DataModel::convertToArtist(const QString &str) {
        return str.right(str.length() - str.indexOf("-") - 1).trimmed();
    }

    QString DataModel::convertToName(const QString &str) {
        return str.left(str.indexOf("-"));
    }

    bool DataModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
        if (!index.isValid())
            return false;

        switch (role) {
            case Qt::DisplayRole: m_list[index.row()].name = value.toString();
                break;
            case Qt::UserRole: m_list[index.row()].artist = value.toString();
                break;
            default: break;
        }
        return QAbstractListModel::setData(index, value, role);
    }
}
