//
// Created by 31305 on 2025/7/16.
//
#include <lyricwidget/lyricwidget.h>

#include <QListView>

namespace Tray::Ui
{

class LyricViewDelegate;
class LyricDataModel;

class LyricWidgetPrivate
{
public:
    QListView *m_listView;
    LyricViewDelegate *m_delegate;
    LyricDataModel *m_dataModel;

};

LyricWidget::LyricWidget(QWidget* parent): QWidget(parent), d(std::make_unique<LyricWidgetPrivate>())
{
    d->m_listView = new QListView(this);
    d->m_listView->setSelectionMode(QAbstractItemView::NoSelection);
}

LyricWidget::~LyricWidget() = default;

}
