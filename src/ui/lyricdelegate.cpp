//
// Created by 31305 on 2025/9/18.
//
#include <QPainter>
#include <lyricwidget/lyricdelegate.h>

namespace Tray::Ui
{

class LyricDelegatePrivate
{
};

LyricDelegate::LyricDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
}

// draw text
void LyricDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    const QRect rect = option.rect;
    const QString text = index.data(Qt::DisplayRole).toString();
    painter->drawText(rect, text, Qt::AlignHCenter | Qt::AlignVCenter);
    painter->restore();
}



}
