#include <configwidget.h>
#include <qwidget.h>
namespace Tray::Ui {
class ConfigWidgetPrivate {
public:
    enum ConfigType { FilePage, ApperancePage, LyricPage, InvalidPage = -1 };

    QList<QWidget*> m_pageArray;
};

ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent) {}

} // namespace Tray::Ui
