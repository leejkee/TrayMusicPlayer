#include <configwidget.h>
#include <filepathconfigwidget.h>

#include <QListWidget>
#include <QStackedWidget>
#include <memory>
#include <QHBoxLayout>


namespace Tray::Ui {
class ConfigWidgetPrivate {
public:
    explicit ConfigWidgetPrivate(QWidget* w);
    enum ConfigType { FilePage, AppearancePage, LyricPage, InvalidPage = -1 };

    QListWidget* m_menuListWidget;
    QStackedWidget* m_stackedWidget;
    FilePathConfigWidget* m_filePathConfigWidget;

    QWidget *q_ptr;
};

ConfigWidgetPrivate::ConfigWidgetPrivate(QWidget *w) : q_ptr(w)
{
    m_menuListWidget = new QListWidget(q_ptr);
    m_stackedWidget = new QStackedWidget(q_ptr);
    m_filePathConfigWidget = new FilePathConfigWidget(q_ptr);
    m_stackedWidget->addWidget(m_filePathConfigWidget);

    auto *configWidgetLayout = new QHBoxLayout;
    configWidgetLayout->addWidget(m_menuListWidget);
    configWidgetLayout->addWidget(m_stackedWidget);
}

ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent) {
    d = std::make_unique<ConfigWidgetPrivate>(this);
    d->m_stackedWidget->addWidget(d->m_filePathConfigWidget);
}

void ConfigWidget::initConfigration()
{
}

} // namespace Tray::Ui
