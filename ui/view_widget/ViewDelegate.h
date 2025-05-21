//
// Created by cww on 25-4-9.
//
#pragma once

#include <QStyledItemDelegate>
#include <QFont>

class QSvgRenderer;


namespace Tray::Ui {
    class ViewDelegate final : public QStyledItemDelegate {
        Q_OBJECT

    public:

        explicit ViewDelegate(QObject *parent = nullptr);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

        void updatePreviousIndex(int index);

        void updatePlayingStatus(bool playable);

        void setCurrentPlaylistMatched(bool b);

    protected:
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                         const QModelIndex &index) override;

    Q_SIGNALS:
        void signalViewItemPlayButtonClicked(int);

        void signalPreviousIndexChanged(int);

        void signalPlayingStatusChanged(bool);

        void signalViewItemOptionsMenu(const QPoint &, const QModelIndex&);

    private:
        static void drawText(QPainter *painter, const QFont &font, const QColor &color, int x, int y,
                             const QString &text);

        int m_previousIndex;

        bool m_isPlaying;

        bool m_renderCurrentPlaylist;

        QModelIndex m_hoverIndex;

        QSvgRenderer *m_svgPlayingRenderer;
        QSvgRenderer *m_svgPauseRenderer;
        QSvgRenderer *m_svgOptionsMenuRender;

        constexpr static int VIEW_BUTTON_SIZE = 15;
        constexpr static int VIEW_PLAY_BUTTON_PADDING = 10;
        constexpr static int VIEW_ADD_BUTTON_PADDING = 30;
        constexpr static int VIEW_TEXT_TOP_PADDING = 18;
        constexpr static int VIEW_TEXT_BOTTOM_PADDING = 7;
        constexpr static int VIEW_TEXT_LEFT_PADDING = VIEW_PLAY_BUTTON_PADDING + VIEW_BUTTON_SIZE + 5;
        constexpr static int NAME_FONT_SIZE = 12;
        constexpr static int ARTIST_FONT_SIZE = 9;

        inline static const auto COLOR_TEXT_NAME = QStringLiteral("#1C1C1E");
        inline static const auto COLOR_TEXT_ARTIST = QStringLiteral("#5E5E5E");
        inline static const auto COLOR_CURRENT_TEXT_NAME = QStringLiteral("#0078D4");
        inline static const auto COLOR_CURRENT_TEXT_ARTIST = QStringLiteral("#005999");
        inline static const auto COLOR_SELECTED_BACKGROUND = QStringLiteral("#E0E0E0");

        inline static const auto FONT_MIRC_HEI = QStringLiteral("Microsoft YaHei");
        inline static const auto TitleFontNormal = QFont(FONT_MIRC_HEI, NAME_FONT_SIZE, QFont::Normal);
        inline static const auto TitleFontBold = QFont(FONT_MIRC_HEI, NAME_FONT_SIZE, QFont::Bold);
        inline static const auto ArtistFontNormal = QFont(FONT_MIRC_HEI, ARTIST_FONT_SIZE, QFont::Normal);
        inline static const auto ArtistFontBold = QFont(FONT_MIRC_HEI, ARTIST_FONT_SIZE, QFont::Bold);
    };
}
