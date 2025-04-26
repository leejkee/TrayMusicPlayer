//
// Created by cww on 25-4-9.
//
#pragma once

#include <QStyledItemDelegate>
#include <QFont>

class QSvgRenderer;


namespace UI::Panel {
    class ViewDelegate final : public QStyledItemDelegate {
        Q_OBJECT

    public:


        explicit ViewDelegate(QObject *parent = nullptr);

        static void drawText(QPainter *painter, const QFont &font, const QColor &color, int x, int y,
                             const QString &text);

        void drawPlayButton(QPainter *painter, const QRect &rect, bool isCurrent) const;

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

        void signalViewItemAddToList(int);

    private:
        int m_previousIndex;
        bool m_isPlaying;
        QSvgRenderer *m_svgPlayingRenderer;
        QSvgRenderer *m_svgPauseRenderer;
        QSvgRenderer *m_svgAddToListRender;

        inline static const auto FONT_MIRC_HEI = QStringLiteral("Microsoft YaHei");

        constexpr static int VIEW_BUTTON_SIZE = 20;

        constexpr static int VIEW_PLAY_BUTTON_PADDING = 10;

        constexpr static int VIEW_ADD_BUTTON_PADDING = 30;

        constexpr static int VIEW_TEXT_TOP_PADDING = 18;

        constexpr static int VIEW_TEXT_BOTTOM_PADDING = 7;

        constexpr static int VIEW_TEXT_LEFT_PADDING = VIEW_PLAY_BUTTON_PADDING + VIEW_BUTTON_SIZE + 5;

        constexpr static int NAME_FONT_SIZE = 12;

        constexpr static int ARTIST_FONT_SIZE = 9;
        // 通用文字色
        inline static const auto COLOR_TEXT_NAME = QStringLiteral("#1C1C1E");           // 标准主文字色（非当前项）
        inline static const auto COLOR_TEXT_ARTIST = QStringLiteral("#5E5E5E");         // 次文字色（非当前项副标题）

        // 当前播放项文字色
        inline static const auto COLOR_CURRENT_TEXT_NAME = QStringLiteral("#0078D4"); // 当前项标题
        inline static const auto COLOR_CURRENT_TEXT_ARTIST = QStringLiteral("#005999"); // 当前项副标题

        // 背景选中色
        inline static const auto COLOR_SELECTED_BACKGROUND = QStringLiteral("#E0E0E0");

        const QFont TitleFontNormal = QFont(FONT_MIRC_HEI, NAME_FONT_SIZE, QFont::Normal);
        const QFont TitleFontBold = QFont(FONT_MIRC_HEI, NAME_FONT_SIZE, QFont::Bold);
        const QFont ArtistFontNormal = QFont(FONT_MIRC_HEI, ARTIST_FONT_SIZE, QFont::Normal);
        const QFont ArtistFontBold = QFont(FONT_MIRC_HEI, ARTIST_FONT_SIZE, QFont::Bold);
    };
}
