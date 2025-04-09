//
// Created by cww on 25-4-9.
//
#pragma once

#include <QStyledItemDelegate>

namespace UI::Panel {
    class ViewDelegate final : public QStyledItemDelegate {
        Q_OBJECT

    public:
        explicit ViewDelegate(QObject *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

        void setPreviousIndex(int index);

        void setPlayStatus(bool playable);

    protected:
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                         const QModelIndex &index) override;


    Q_SIGNALS:
        void signalViewPlayButtonClick(int);

        void signalPlayToggle();

        void signalPreviousIndexChanged(int);

        void signalPlayingStatusChanged(bool);

    private:
        int m_previousIndex;
        bool m_isPlaying;
    };
}
