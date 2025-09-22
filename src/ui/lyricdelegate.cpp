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
    int m_index;
};

LyricDelegate::LyricDelegate(QObject* parent) : QStyledItemDelegate(parent), d(std::make_unique<LyricDelegatePrivate>())
{
}

void LyricDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    const QRect rect = option.rect;
    const QString text = index.data(Qt::DisplayRole).toString();

    if (index.row() == d->m_index)
    {
    }

    painter->restore();
}

bool LyricDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
}

}
