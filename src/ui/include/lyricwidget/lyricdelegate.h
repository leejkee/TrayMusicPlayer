//
// Created by 31305 on 2025/9/16.
//
#pragma once
#include <QStyledItemDelegate>
#include <memory>

namespace Tray::Ui
{

class LyricDelegatePrivate;
class LyricDelegate final : public QStyledItemDelegate
{
    public:
    explicit LyricDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    static void drawText(QPainter* painter
                         , const QFont& font
                         , const QColor& color
                         , const QRect& rect
                         , const QString& text);

protected:

public Q_SLOTS:
    void setCurrentLineIndex(int line);

private:
    std::unique_ptr<LyricDelegatePrivate> d;
};

}
