//
// Created by cww on 25-4-10.
//
#include "../widgets/viewdelegate.h"
#include <uitools.h>
#include <traysvg.h>
#include <QListView>
#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QApplication>


namespace Tray::Ui {
    ViewDelegate::ViewDelegate(QObject *parent) : QStyledItemDelegate(parent)
                                                  , m_previousIndex(UNINITIALIZED_VALUE)
                                                  , m_isPlaying(false)
                                                  , m_renderCurrentPlaylist(false)
                                                  , m_hoverIndex({})
                                                  , m_svgPlayingRenderer(new QSvgRenderer(Res::ViewPlaySVG))
                                                  , m_svgPauseRenderer(new QSvgRenderer(Res::ViewPauseSVG))
                                                  , m_svgOptionsMenuRender(new QSvgRenderer(Res::OptionsMenuSVG)) {
    }

    void ViewDelegate::drawText(QPainter *painter, const QFont &font, const QColor &color, const int x, const int y,
                                const QString &text) {
        painter->save();
        painter->setPen(QPen(color, Qt::SolidLine));
        painter->setFont(font);
        painter->drawText(x, y, text);
        painter->restore();
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
        } else if (option.state & QStyle::State_MouseOver) {
            painter->fillRect(rect, COLOR_SELECTED_BACKGROUND);
        }
        // Play/pause
        const QRect buttonPlayRect(rect.left() + VIEW_PLAY_BUTTON_PADDING,
                                   rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                   VIEW_BUTTON_SIZE,
                                   VIEW_BUTTON_SIZE);

        // add song to the specified playlist
        const QRect buttonOptionsRect(rect.right() - VIEW_ADD_BUTTON_PADDING,
                                      rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                      VIEW_BUTTON_SIZE,
                                      VIEW_BUTTON_SIZE);

        const bool isHovered = (option.state & QStyle::State_MouseOver);
        const bool isCurrent = (index.row() == m_previousIndex);

        // draw Buttons
        if (isHovered) {
            m_svgOptionsMenuRender->render(painter, buttonOptionsRect);
            if (isCurrent && m_renderCurrentPlaylist) {
                if (m_isPlaying) {
                    m_svgPauseRenderer->render(painter, buttonPlayRect);
                } else {
                    m_svgPlayingRenderer->render(painter, buttonPlayRect);
                }
            } else {
                m_svgPlayingRenderer->render(painter, buttonPlayRect);
            }
        } else {
            if (isCurrent && m_renderCurrentPlaylist) {
                if (m_isPlaying) {
                    m_svgPauseRenderer->render(painter, buttonPlayRect);
                } else {
                    m_svgPlayingRenderer->render(painter, buttonPlayRect);
                }
            }
        }

        // draw Text
        if (isCurrent && m_renderCurrentPlaylist) {
            drawText(painter, TitleFontBold, COLOR_CURRENT_TEXT_NAME,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + VIEW_TEXT_TOP_PADDING,
                     title);

            drawText(painter, ArtistFontBold, COLOR_CURRENT_TEXT_ARTIST,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + rect.height() - VIEW_TEXT_BOTTOM_PADDING,
                     artist);
        } else {
            drawText(painter, TitleFontNormal, COLOR_TEXT_NAME,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + VIEW_TEXT_TOP_PADDING,
                     title);

            drawText(painter, ArtistFontNormal, COLOR_TEXT_ARTIST,
                     rect.left() + VIEW_TEXT_LEFT_PADDING,
                     rect.top() + rect.height() - VIEW_TEXT_BOTTOM_PADDING,
                     artist);
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

        const QRect buttonOptionsRect(option.rect.right() - VIEW_ADD_BUTTON_PADDING,
                                      option.rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                      VIEW_BUTTON_SIZE,
                                      VIEW_BUTTON_SIZE);
        const bool isCurrent = (index.row() == m_previousIndex);

        switch (event->type()) {
            case QEvent::MouseMove: {
                const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
                if (!mouseEvent) {
                    break;
                }
                const auto pos = mouseEvent->pos();

                // --- Hover Tracking and Repaint ---
                if (auto *view = qobject_cast<QListView *>(this->parent())) {
                    if (const QModelIndex currentIndex = view->indexAt(pos); currentIndex != m_hoverIndex) {
                        if (m_hoverIndex.isValid()) {
                            view->update(m_hoverIndex);
                        }
                        if (currentIndex.isValid()) {
                            view->update(currentIndex);
                        }
                        m_hoverIndex = currentIndex;
                    }
                }

                // --- Cursor Handling ---
                const bool overPlay = buttonPlayRect.contains(pos);
                const bool overOptions = buttonOptionsRect.contains(pos);
                if (const bool isHoveredMouse = (index == m_hoverIndex);
                    (isCurrent || isHoveredMouse) && (overPlay || overOptions)) {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                } else {
                    QApplication::restoreOverrideCursor();
                }
                break;
            }
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease: {
                const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
                if (!mouseEvent) break;
                const auto mousePos = mouseEvent->pos();

                if (const bool isHoveredMouse = (index == m_hoverIndex); isCurrent || isHoveredMouse) {
                    // cursor is on the item
                    if (buttonPlayRect.contains(mousePos)) {
                        if (event->type() == QEvent::MouseButtonRelease) {
                            Q_EMIT signalViewItemPlayButtonClicked(index.row());
                        }
                        return true;
                    }
                    if (buttonOptionsRect.contains(mousePos)) {
                        if (event->type() == QEvent::MouseButtonRelease) {
                            Q_EMIT signalViewItemOptionsMenu(mousePos, index);
                        }
                        return true;
                    }
                }
                break;
            }
            case QEvent::Leave: {
                QApplication::restoreOverrideCursor();
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

    void ViewDelegate::setCurrentPlaylistMatched(const bool b) {
        m_renderCurrentPlaylist = b;
    }
}
