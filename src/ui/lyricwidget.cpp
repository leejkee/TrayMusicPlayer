//
// Created by 31305 on 2025/7/16.
//
#include <lyricwidget/lyricwidget.h>
#include <lyricwidget/lyricdelegate.h>
#include <lyricwidget/lyricmodel.h>
#include <log/log.h>
#include <QListView>

namespace Tray::Ui
{

class LyricWidgetPrivate
{
public:
    QListView *m_listView;
    LyricModel *m_lyricModel;
    LyricDelegate *m_viewDelegate;
    int m_currentTextIndex{0};
};

LyricWidget::LyricWidget(QWidget* parent): QWidget(parent), d(std::make_unique<LyricWidgetPrivate>())
{
    d->m_lyricModel = new LyricModel(this);
    d->m_viewDelegate = new LyricDelegate(this);
    d->m_listView = new QListView(this);
    d->m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    d->m_listView->setItemDelegate(d->m_viewDelegate);
    d->m_listView->setModel(d->m_lyricModel);
}

void LyricWidget::updateCurrentTiming(const int index)
{
    if (index >= 0 && index < d->m_lyricModel->rowCount())
    {
        d->m_currentTextIndex = index;
    }
    else
    {
        LOG_ERROR(QString("Out of range index: [%1]").arg(index));
    }
}

void LyricWidget::updateLyric(const QStringList& lyricText, const QList<int64_t>& lyricsTiming)
{
    d->m_lyricModel->setLyric(lyricText, lyricsTiming);
    updateCurrentTiming(0);
}

int LyricWidget::currentIndex() const
{
    return d->m_currentTextIndex;
}

LyricWidget::~LyricWidget() = default;

}
