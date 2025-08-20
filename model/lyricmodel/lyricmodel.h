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

    /// @param lyricsText the text of lyric line
    /// @param lyricsTiming the timing of lyric line
    /// Update the lyrics when a new MP3 file with a same-name LRC file is loaded.
    void LyricModel::setLyric(const QStringList& lyricsText, const QList<int64_t>& lyricsTiming);

protected:
    [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

private:
    std::unique_ptr<LyricModelPrivate> d;
};
}
