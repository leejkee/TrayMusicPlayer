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

protected:

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
    std::unique_ptr<LyricDelegatePrivate> d;

};

}
