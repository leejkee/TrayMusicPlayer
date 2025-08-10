//
// Created by 31305 on 2025/8/8.
//
#pragma once
#include <memory>
#include <QAbstractListModel>

namespace Tray::Model
{

class LyricModelPrivate;
class LyricModel final: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LyricModel(QObject *parent = nullptr);

    void LyricModel::setLyric(const QStringList& lyricsText, const QList<int64_t>& lyricsTiming);

protected:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

private:
    std::unique_ptr<LyricModelPrivate> d;
};
}
