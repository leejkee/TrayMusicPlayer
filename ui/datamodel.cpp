//
// Created by cww on 25-4-10.
//
#include "datamodel.h"

namespace Tray::Ui {
    DataModel::DataModel(const QStringList &list, QObject *parent): QAbstractListModel(parent), m_list(list) {
    }

    void DataModel::setMusicList(const QStringList &list) {
        beginResetModel();
        m_list.clear();
        m_list = list;
        endResetModel();
    }

    int DataModel::rowCount(const QModelIndex &parent) const {
        return parent.isValid() ? 0 : static_cast<int>(m_list.size());
    }

    QVariant DataModel::data(const QModelIndex &index, const int role) const {
        if (!index.isValid() || index.row() >= m_list.size()) return {};
        const auto str = m_list[index.row()];
        switch (role) {
            case Qt::DisplayRole: return convertToName(str);
            case Qt::UserRole: return convertToArtist(str);
            case Qt::UserRole + 1: return str;
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
        Q_UNUSED(role);
        Q_UNUSED(index);
        Q_UNUSED(value);
        return false;
    }
}
