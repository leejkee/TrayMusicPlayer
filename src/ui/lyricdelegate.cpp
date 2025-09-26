//
// Created by 31305 on 2025/9/18.
//
#include <QPainter>
#include <lyricwidget/lyricdelegate.h>

namespace Tray::Ui
{

class LyricDelegatePrivate
{
public:
    static constexpr int LINE_HEIGHT_NORMAL = 20;
    static constexpr int LINE_HEIGHT_CURRENT = 20;
    int currentLineIndex = -1;
    inline static const auto FONT_MIRC_HEI = QStringLiteral("Microsoft YaHei");
    inline static const auto NormalLineFont = QFont(FONT_MIRC_HEI
                                                         , 10
                                                         , QFont::Normal);
    inline static const auto HighlightLineFont = QFont(FONT_MIRC_HEI
                                                         , 12
                                                         , QFont::Bold);
};

LyricDelegate::LyricDelegate(QObject* parent) : QStyledItemDelegate(parent), d(std::make_unique<LyricDelegatePrivate>())
{
}

QSize LyricDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.row() == d->currentLineIndex)
    {
        return {-1, LyricDelegatePrivate::LINE_HEIGHT_CURRENT};
    }
    return {-1, LyricDelegatePrivate::LINE_HEIGHT_NORMAL};
}

void LyricDelegate::drawText(QPainter* painter
                            , const QFont& font
                            , const QColor& color
                            , const QRect& rect
                            , const QString& text)
{
    painter->save();
    painter->setPen(QPen(color, Qt::SolidLine));
    painter->setFont(font);
    painter->drawText(rect, text, Qt::AlignVCenter | Qt::AlignHCenter);
    painter->restore();
}

// draw text
void LyricDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QRect rect = option.rect;
    const QString text = index.data(Qt::DisplayRole).toString();
    if (index.row() == d->currentLineIndex)
    {
        drawText(painter, LyricDelegatePrivate::HighlightLineFont, QColor(), rect, text);
    }
    else
    {
        drawText(painter, LyricDelegatePrivate::NormalLineFont, QColor(), rect, text);
    }
}

void LyricDelegate::setCurrentLineIndex(const int lineIndex)
{
    if (d->currentLineIndex == lineIndex)
    {
        return;
    }
    d->currentLineIndex = lineIndex;
}

int LyricDelegate::currentLineIndex() const
{
    return d->currentLineIndex;
}

}
