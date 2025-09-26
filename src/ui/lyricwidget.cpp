//
// Created by 31305 on 2025/7/16.
//
#include <lyricwidget/lyricwidget.h>
#include <lyricwidget/lyricdelegate.h>
#include <lyricwidget/lyricmodel.h>
#include <stylebutton/stylebutton.h>
#include <lyricwidget/lyricview.h>
#include <log/log.h>
#include <traysvg.h>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>

namespace Tray::Ui
{
class LyricWidgetPrivate
{
public:
    LyricView* m_listView;
    LyricModel* m_lyricModel;
    LyricDelegate* m_viewDelegate;
    QVBoxLayout* m_layout;
    QLabel* m_titleLabel;
    bool m_isUserInteracting{false};
    Panel::StyleButton* m_backButton;

    inline static auto INFO_NO_LRC = QStringLiteral("Lyric file not found: ");
};


LyricWidget::LyricWidget(QWidget* parent)
    : QWidget(parent),
      d(std::make_unique<LyricWidgetPrivate>())
{
    d->m_lyricModel = new LyricModel(this);
    d->m_viewDelegate = new LyricDelegate(this);
    d->m_listView = new LyricView(this);
    d->m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    d->m_listView->setItemDelegate(d->m_viewDelegate);
    d->m_listView->setModel(d->m_lyricModel);
    d->m_layout = new QVBoxLayout;
    d->m_backButton = new Panel::StyleButton({}
                                             , {30, 30}
                                             , Res::DownSVG
                                             , {}
                                             , this);
    d->m_titleLabel = new QLabel(this);
    d->m_titleLabel->setAlignment(Qt::AlignCenter);
    d->m_titleLabel->setFixedHeight(30);

    auto* topLayout = new QHBoxLayout;
    topLayout->addWidget(d->m_backButton);
    topLayout->addWidget(d->m_titleLabel);
    d->m_layout->setContentsMargins(0, 0, 0, 0);
    d->m_layout->setSpacing(2);
    d->m_layout->addItem(topLayout);
    d->m_layout->addWidget(d->m_listView);
    setLayout(d->m_layout);
    connect(d->m_backButton
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalBackButtonClicked();
            });

    connect(d->m_listView
            , &LyricView::signalScrollToCenter
            , this
            , &LyricWidget::handleAutoScrollToCenter);
}

void LyricWidget::updateCurrentTiming(const int index)
{
    if (index >= 0 && index < d->m_lyricModel->rowCount())
    {
        d->m_viewDelegate->setCurrentLineIndex(index);
        d->m_listView->update();
        if (d->m_listView->isActiveAutoScroll())
        {
            handleAutoScrollToCenter();
        }
    }
    else
    {
        LOG_ERROR(QString("Out of range index: [%1]").arg(index));
    }
}

void LyricWidget::updateLyric(const QString& musicTitle
                              , const QStringList& lyricText
                              , const QList<int64_t>& lyricsTiming)
{
    QString title = musicTitle;
    if (lyricText.isEmpty())
    {
        title = LyricWidgetPrivate::INFO_NO_LRC + title;
    }
    d->m_titleLabel->setText(title);
    d->m_lyricModel->setLyric(lyricText, lyricsTiming);
    updateCurrentTiming(0);
}


void LyricWidget::handleAutoScrollToCenter()
{
    const auto modelindex = d->m_lyricModel->
                               index(d->m_viewDelegate->currentLineIndex(), 0);
    d->m_listView->scrollTo(modelindex, QAbstractItemView::PositionAtCenter);
}

LyricWidget::~LyricWidget() = default;
}
