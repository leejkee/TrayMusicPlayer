//
// Created by cww on 25-4-9.
//
#pragma once

#include <QStyledItemDelegate>

class QSvgRenderer;


namespace UI::Panel {
    class ViewDelegate final : public QStyledItemDelegate {
        Q_OBJECT

    public:
        inline static const auto FONT_MIRC_HEI = QStringLiteral("Microsoft YaHei");

        constexpr static int VIEW_BUTTON_SIZE = 20;

        constexpr static int VIEW_PLAY_BUTTON_PADDING = 10;

        constexpr static int VIEW_ADD_BUTTON_PADDING = 30;

        constexpr static int VIEW_TEXT_TOP_PADDING = 18;

        constexpr static int VIEW_TEXT_BOTTOM_PADDING = 7;

        constexpr static int VIEW_TEXT_LEFT_PADDING = VIEW_PLAY_BUTTON_PADDING + VIEW_BUTTON_SIZE + 5;

        constexpr static int NAME_FONT_SIZE = 12;

        constexpr static int ARTIST_FONT_SIZE = 9;

        explicit ViewDelegate(QObject *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

        void updatePreviousIndex(int index);

        void updatePlayingStatus(bool playable);

    protected:
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                         const QModelIndex &index) override;

    Q_SIGNALS:
        void signalViewItemPlayButtonClicked(int);

        void signalPreviousIndexChanged(int);

        void signalPlayingStatusChanged(bool);

    private:
        int m_previousIndex;
        bool m_isPlaying;
        QSvgRenderer *m_svgPlayingRenderer;
        QSvgRenderer *m_svgPauseRenderer;
        QSvgRenderer *m_svgAddToListRender;
    };
}
