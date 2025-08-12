//
// Created by 31305 on 2025/8/8.
//
#include <lyricmodel.h>
#include <lyricparser.h>

namespace Tray::Model
{

class LyricModelPrivate
{
public:
    Badfish::AudioToolkit::LyricParser* m_parser;
    QStringList m_lyricsText;
    QList<int64_t> m_lyricsTiming;
};

LyricModel::LyricModel(QObject* parent) : QAbstractListModel(parent), d(std::make_unique<LyricModelPrivate>())
{
}

/// @param lyricsText the text of lyric line
/// @param lyricsTiming the timing of lyric line
/// Update the lyrics when a new MP3 file with a same-name LRC file is loaded.
void LyricModel::setLyric(const QStringList& lyricsText, const QList<int64_t>& lyricsTiming)
{
    beginResetModel();
    d->m_lyricsText = lyricsText;
    d->m_lyricsTiming = lyricsTiming;
    endResetModel();
}

QVariant LyricModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }
    const int64_t lyricTiming = d->m_lyricsTiming.at(index.row());
    const QString lyricText = d->m_lyricsText.at(index.row());
    switch (role)
    {
        case Qt::DisplayRole: return lyricText;
        case Qt::UserRole: return lyricTiming;
        default: return {};
    }
}

int LyricModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return static_cast<int>(d->m_lyricsText.size());
}

}
