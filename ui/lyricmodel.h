//
// Created by 31305 on 2025/7/19.
//

#pragma once
#include <QAbstractListModel>

class LyricModel final: public QAbstractListModel
{
public:
    explicit LyricModel(QObject *parent = nullptr);

    void setLyricData();

protected:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:

};

