#include <configwidget.h>
#include <filepathconfigwidget.h>

#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <memory>
#include <qlistwidget.h>
#include <qstackedwidget.h>
#include <qwidget.h>

namespace Tray::Ui {
class ConfigWidgetPrivate {
public:
    ConfigWidgetPrivate(QWidget* q_ptr);
    enum ConfigType { FilePage, ApperancePage, LyricPage, InvalidPage = -1 };

    QListWidget* m_menuListWidget;
    QStackedWidget* m_stackedWidget;

    FilePathConfigWidget* m_filePathConfigWidget;
};

ConfigWidgetPrivate::ConfigWidgetPrivate(QWidget *q_ptr)
{
    m_menuListWidget = new QListWidget(q_ptr);
    m_stackedWidget = new QStackedWidget(q_ptr);
    m_filePathConfigWidget = new FilePathConfigWidget(q_ptr);
}

ConfigWidget::ConfigWidget(QWidget* parent) : QDialog(parent) {

    d = std::make_unique<ConfigWidgetPrivate>(this);
    d->m_stackedWidget->addWidget(d->m_filePathConfigWidget);


}

void ConfigWidget::initConfigration()
{


}

} // namespace Tray::Ui
