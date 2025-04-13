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
                                                  , m_isPlaying(false) {
        connect(this, &ViewDelegate::signalPreviousIndexChanged, this, [this](const int index) {
            if (auto *view = qobject_cast<QListView *>(this->parent())) {
                qDebug() << "Song index: " << index;
                if (m_previousIndex >= 0) {
                    view->update(view->model()->index(m_previousIndex, 0));
                }
                if (index >= 0) {
                    view->update(view->model()->index(index, 0));
                }
            }
        });

        connect(this, &ViewDelegate::signalPlayingStatusChanged, this, [this](const bool b) {
            qDebug() << "signalPlayingStatusChanged emitted. b =" << b << "m_previousIndex =" << m_previousIndex;
            Q_UNUSED(b);
            if (auto *view = qobject_cast<QListView *>(this->parent())) {
                if (m_previousIndex >= 0) {
                    qDebug() << "signalPlayingStatusChanged:" << m_previousIndex;
                    view->update(view->model()->index(m_previousIndex, 0));
                }
            }
        });
    }

    void ViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {
        painter->save();

        const QRect rect = option.rect;
        const QString title = index.data(Qt::DisplayRole).toString();
        const QString artist = index.data(Qt::UserRole).toString();

        // font
        const QFont titleFont(FONT_MIRC_HEI, 12, QFont::Normal);
        const QFont artistFont(FONT_MIRC_HEI, 9);

        const int coverSize = rect.height() - 2 * VIEW_LOGO_PADDING;

        // background
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(rect, QColor(224, 224, 224)); // 选中背景颜色
        }


        // name of song
        painter->setFont(titleFont);
        painter->drawText(rect.left() + coverSize + 8 * VIEW_LOGO_PADDING,
                          rect.top() + VIEW_LOGO_PADDING * 4, title);

        // Artist
        painter->setFont(artistFont);
        painter->setPen(Qt::gray);
        painter->drawText(rect.left() + coverSize + 8 * VIEW_LOGO_PADDING,
                          rect.top() + VIEW_LOGO_PADDING * 7, artist);

        // Play/pause
        const QRect buttonPlayRect(rect.left() + VIEW_LOGO_PADDING,
                                   rect.center().y() - VIEW_BUTTON_SIZE / 2,
                                   VIEW_BUTTON_SIZE,
                                   VIEW_BUTTON_SIZE);

        const QRect buttonRect(rect.right() - 80,
                               rect.center().y() - VIEW_BUTTON_SIZE / 2,
                               VIEW_BUTTON_SIZE,
                               VIEW_BUTTON_SIZE);

        static QSvgRenderer svgAddToListRender(SvgRes::AdddSVG);
        svgAddToListRender.render(painter, buttonRect);

        static QSvgRenderer svgPlayingRenderer(SvgRes::ViewPlaySVG);
        static QSvgRenderer svgPauseRenderer(SvgRes::ViewPauseSVG);

        if (index.row() != m_previousIndex) {
            svgPlayingRenderer.render(painter, buttonPlayRect);
        } else {
            if (m_isPlaying) {
                svgPauseRenderer.render(painter, buttonPlayRect);
            } else {
                svgPlayingRenderer.render(painter, buttonPlayRect);
            }
        }

        painter->restore();
    }

    bool ViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                   const QModelIndex &index) {
        if (!index.isValid()) return false;

        const QRect buttonRect(option.rect.left() + VIEW_LOGO_PADDING,
                               option.rect.center().y() - VIEW_BUTTON_SIZE / 2,
                               VIEW_BUTTON_SIZE,
                               VIEW_BUTTON_SIZE);

        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
            if (const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event); buttonRect.contains(mouseEvent->pos())) {
                if (event->type() == QEvent::MouseButtonRelease) {
                    if (index.row() == m_previousIndex) {
                        m_isPlaying = !m_isPlaying;
                        Q_EMIT signalPlayToggle();
                        qDebug() << "signalPlayTg";
                    } else {
                        m_previousIndex = index.row();
                        Q_EMIT signalViewPlayButtonClick(index.row());
                        qDebug() << "signalViewPlayButtonClick";
                    }
                }
                return true;
            }
        }
        return false;
    }

    void ViewDelegate::setPreviousIndex(const int index) {
        if (index != m_previousIndex) {
            m_previousIndex = index;
            Q_EMIT signalPreviousIndexChanged(index);
        }
    }

    void ViewDelegate::setPlayStatus(const bool playable) {
        if (playable != m_isPlaying) {
            qDebug() << "ViewDelegate::setPlayStatus" << playable;
            m_isPlaying = playable;
            Q_EMIT signalPlayingStatusChanged(playable);
        }
    }
}
