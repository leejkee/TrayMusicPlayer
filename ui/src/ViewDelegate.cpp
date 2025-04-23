//
// Created by cww on 25-4-10.
//
#include <panel/ViewDelegate.h>
#include <ui/Assets.h>
#include <QListView>
#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>


namespace UI::Panel {
    ViewDelegate::ViewDelegate(QObject *parent) : QStyledItemDelegate(parent)
                                                  , m_previousIndex(UNINITIALIZED_VALUE)
                                                  , m_isPlaying(false)
                                                  , m_svgPlayingRenderer(new QSvgRenderer(SvgRes::ViewPlaySVG))
                                                  , m_svgPauseRenderer(new QSvgRenderer(SvgRes::ViewPauseSVG))
                                                  , m_svgAddToListRender(new QSvgRenderer(SvgRes::AdddSVG)) {
    }

    void ViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
        painter->save();

        const QRect rect = option.rect;
        const QString title = index.data(Qt::DisplayRole).toString();
        const QString artist = index.data(Qt::UserRole).toString();

        // font
        const QFont titleFont(FONT_MIRC_HEI, NAME_FONT_SIZE, QFont::Normal);
        const QFont artistFont(FONT_MIRC_HEI, ARTIST_FONT_SIZE, QFont::Bold);

        // background
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(rect, QColor(224, 224, 224));
        }

        // name of song
        painter->setFont(titleFont);
        painter->drawText(rect.left() + VIEW_TEXT_LEFT_PADDING,
                          rect.top() + VIEW_TEXT_TOP_PADDING, title);

        // Artist
        painter->setFont(artistFont);
        painter->setPen(Qt::gray);
        painter->drawText(rect.left() + VIEW_TEXT_LEFT_PADDING,
                          rect.top() + rect.height() - VIEW_TEXT_BOTTOM_PADDING, artist);

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
            m_svgPlayingRenderer->render(painter, buttonPlayRect);
        } else {
            if (m_isPlaying) {
                m_svgPauseRenderer->render(painter, buttonPlayRect);
            } else {
                m_svgPlayingRenderer->render(painter, buttonPlayRect);
            }
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

        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
            const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event);
            if (buttonPlayRect.contains(mouseEvent->pos())) {
                if (event->type() == QEvent::MouseButtonRelease) {
                    Q_EMIT signalViewItemPlayButtonClicked(index.row());
                }
            }
            if (buttonAddToListRect.contains(mouseEvent->pos())) {
                if (event->type() == QEvent::MouseButtonRelease) {
                    Q_EMIT signalViewItemAddToList(index.row());
                }
            }
            return true;
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
