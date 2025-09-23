//
// Created by 31305 on 2025/7/16.
//
#include <lyricwidget/lyricwidget.h>
#include <lyricwidget/lyricdelegate.h>
#include <lyricwidget/lyricmodel.h>
#include <stylebutton/stylebutton.h>
#include <log/log.h>
#include <QListView>
#include <QVBoxLayout>

namespace Tray::Ui
{
class LyricWidgetPrivate
{
public:
    QListView* m_listView;
    LyricModel* m_lyricModel;
    LyricDelegate* m_viewDelegate;
    QVBoxLayout* m_layout;
    Panel::StyleButton* m_backButton;
    int m_currentLineIndex{0};
};

LyricWidget::LyricWidget(QWidget* parent)
    : QWidget(parent),
      d(std::make_unique<LyricWidgetPrivate>())
{
    d->m_lyricModel = new LyricModel(this);
    d->m_viewDelegate = new LyricDelegate(this);
    d->m_listView = new QListView(this);
    d->m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    d->m_listView->setItemDelegate(d->m_viewDelegate);
    d->m_listView->setModel(d->m_lyricModel);
    d->m_layout = new QVBoxLayout;
    d->m_backButton = new Panel::StyleButton({"↓"}, {30, 30}, {}, {}, this);
    auto* spaceH = new QSpacerItem(-1
                                   , 0
                                   , QSizePolicy::Expanding
                                   , QSizePolicy::Minimum);
    auto* topLayout = new QHBoxLayout;
    topLayout->addWidget(d->m_backButton);
    topLayout->addItem(spaceH);
    d->m_layout->setContentsMargins(0, 0, 0, 0);
    d->m_layout->setSpacing(0);
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
}

void LyricWidget::updateCurrentTiming(const int index)
{
    if (index >= 0 && index < d->m_lyricModel->rowCount())
    {
        d->m_currentLineIndex = index;
    }
    else
    {
        LOG_ERROR(QString("Out of range index: [%1]").arg(index));
    }
}

void LyricWidget::updateLyric(const QStringList& lyricText
                              , const QList<int64_t>& lyricsTiming)
{
    d->m_lyricModel->setLyric(lyricText, lyricsTiming);
    updateCurrentTiming(0);
}

int LyricWidget::currentIndex() const
{
    return d->m_currentLineIndex;
}


LyricWidget::~LyricWidget() = default;
}
