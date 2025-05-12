//
// Created by cww on 25-4-10.
//
#include "ViewDelegate.h"
#include <UiConfig.h>
#include <TraySVG.h>
#include <QListView>
#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QApplication>


namespace Tray::Ui::Panel {
    ViewDelegate::ViewDelegate(QObject *parent) : QStyledItemDelegate(parent)
                                                  , m_previousIndex(UNINITIALIZED_VALUE)
                                                  , m_isPlaying(false)
                                                  , m_svgPlayingRenderer(new QSvgRenderer(Res::ViewPlaySVG))
                                                  , m_svgPauseRenderer(new QSvgRenderer(Res::ViewPauseSVG))
                                                  , m_svgAddToListRender(new QSvgRenderer(Res::AdddSVG)) {
    }

    void ViewDelegate::drawText(QPainter *painter, const QFont &font, const QColor &color, const int x, const int y,
                                const QString &text) {
        painter->save();
        painter->setPen(QPen(color, Qt::SolidLine));
        painter->setFont(font);
        painter->drawText(x, y, text);
        painter->restore();
    }

    void ViewDelegate::drawPlayButton(QPainter *painter, const QRect &rect, const bool isCurrent) const {
        if (isCurrent) {
            if (m_isPlaying) {
                m_svgPauseRenderer->render(painter, rect);
            } else {
                m_svgPlayingRenderer->render(painter, rect);
            }
        } else {
            m_svgPlayingRenderer->render(painter, rect);
        }
    }

    void ViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
        painter->save();

        const QRect rect = option.rect;
        const QString title = index.data(Qt::DisplayRole).toString();
        const QString artist = index.data(Qt::UserRole).toString();

        // background
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(rect, COLOR_SELECTED_BACKGROUND);
        }

        // Play/pause
        const QRect buttonPlayRect(rect.left() + VIEW_PLAY_BUTTON_PADDING,
                                   rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                   VIEW_BUTTON_SIZE,
                                   VIEW_BUTTON_SIZE);

        const QRect buttonAddToListRect(rect.right() - VIEW_ADD_BUTTON_PADDING,
                                        rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                        VIEW_BUTTON_SIZE,
                                        VIEW_BUTTON_SIZE);

        m_svgAddToListRender->render(painter, buttonAddToListRect);
        if (index.row() != m_previousIndex) {
            drawText(painter, TitleFontNormal, COLOR_TEXT_NAME,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + VIEW_TEXT_TOP_PADDING,
                     title);

            drawText(painter, ArtistFontNormal, COLOR_TEXT_ARTIST,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + rect.height() - VIEW_TEXT_BOTTOM_PADDING,
                     artist);

            drawPlayButton(painter, buttonPlayRect, false);
        } else {
            drawText(painter, TitleFontBold, COLOR_CURRENT_TEXT_NAME,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + VIEW_TEXT_TOP_PADDING,
                     title);

            drawText(painter, ArtistFontBold, COLOR_CURRENT_TEXT_ARTIST,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + rect.height() - VIEW_TEXT_BOTTOM_PADDING,
                     artist);

            drawPlayButton(painter, buttonPlayRect, true);
        }
        painter->restore();
    }

    bool ViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) {
        if (!index.isValid()) return false;

        const QRect buttonPlayRect(option.rect.left() + VIEW_PLAY_BUTTON_PADDING,
                                   option.rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                   VIEW_BUTTON_SIZE,
                                   VIEW_BUTTON_SIZE);

        const QRect buttonAddToListRect(option.rect.right() - VIEW_ADD_BUTTON_PADDING,
                                        option.rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                        VIEW_BUTTON_SIZE,
                                        VIEW_BUTTON_SIZE);
        switch (event->type()) {
            case QEvent::MouseMove: {
                const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
                const auto pos = mouseEvent->pos();
                if (!mouseEvent) return false;

                const bool overPlay = buttonPlayRect.contains(pos);
                const bool overAdd = buttonAddToListRect.contains(pos);

                if (overPlay || overAdd) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                } else {
                    QApplication::restoreOverrideCursor();
                }
                break;
            }
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease: {
                const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
                if (!mouseEvent) return false;
                const auto pos = mouseEvent->pos();

                if (buttonPlayRect.contains(pos)) {
                    if (event->type() == QEvent::MouseButtonRelease) {
                        Q_EMIT signalViewItemPlayButtonClicked(index.row());
                    }
                    return true;
                }
                if (buttonAddToListRect.contains(pos)) {
                    if (event->type() == QEvent::MouseButtonRelease) {
                        Q_EMIT signalViewItemAddToList(pos, index.row());
                    }
                    return true;
                }
                break;
            }
            default:
                break;
        }
        return false;
    }

    void ViewDelegate::updatePreviousIndex(const int index) {
        if (index != m_previousIndex) {
            if (auto *view = qobject_cast<QListView *>(this->parent())) {
                if (m_previousIndex >= 0) {
                    view->update(view->model()->index(m_previousIndex, 0));
                }
                if (index >= 0) {
                    view->update(view->model()->index(index, 0));
                }
            }
            m_previousIndex = index;
        }
    }

    void ViewDelegate::updatePlayingStatus(const bool playable) {
        if (playable != m_isPlaying) {
            m_isPlaying = playable;
            if (auto *view = qobject_cast<QListView *>(this->parent())) {
                if (m_previousIndex >= 0) {
                    view->update(view->model()->index(m_previousIndex, 0));
                }
            }
        }
    }
}
