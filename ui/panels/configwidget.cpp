#include <configwidget.h>
#include <betterbutton.h>
#include <QLogger.h>
#include <memory>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QLabel>


namespace Tray::Ui::Panel {
class ConfigWidgetPrivate {
public:
    QStackedWidget* m_stackedWidget;
    QLabel *m_titleLabel;
    std::unique_ptr<QHash<QString, QWidget*>> m_actionMap;
    QHBoxLayout* m_mainLayout;
    QVBoxLayout* m_buttonLayout;
    QVBoxLayout* m_areaLayout;
    Log::QLogger Log;
};


ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent), d(std::make_unique<ConfigWidgetPrivate>()){
    setObjectName("ConfigWidget");
    d->Log = Log::QLogger(this->objectName());
    d->m_actionMap = std::make_unique<QHash<QString, QWidget*>>();
    d->m_stackedWidget = new QStackedWidget(this);
    d->m_titleLabel = new QLabel(this);
    d->m_buttonLayout = new QVBoxLayout;
    d->m_areaLayout = new QVBoxLayout;
    d->m_mainLayout = new QHBoxLayout;

    d->m_areaLayout->addWidget(d->m_titleLabel);
    d->m_areaLayout->addWidget(d->m_stackedWidget);

    d->m_mainLayout->addLayout(d->m_buttonLayout);
    d->m_mainLayout->addLayout(d->m_areaLayout);
    setLayout(d->m_mainLayout);
}

ConfigWidget::~ConfigWidget() = default;


void ConfigWidget::addConfigWidget(const QString &actionName, QWidget *w) {
    addConfigWidget({actionName, {}, {}, 0, 0}, w);
}


void ConfigWidget::addConfigWidget(const BetterButtonMetaData &actionInfo, QWidget *w) {
    if (w == nullptr) {
        d->Log.log(Log::QLogger::LogLevel::Warning, "Null widget, add nothing to ConfigWidget");
        return;
    }
    auto* actionButton = new BetterButton(actionInfo, this);

    if (d->m_buttonLayout->count() == 0) {
        d->m_buttonLayout->addWidget(actionButton);
        d->m_buttonLayout->addStretch();
    }
    else {
        d->m_buttonLayout->insertWidget(d->m_buttonLayout->count() - 1, actionButton);
    }
    d->m_actionMap->insert(actionInfo.name, w);
    d->m_stackedWidget->addWidget(w);

    if (d->m_stackedWidget->count() == 1) {
        showActionArea(actionInfo.name);
    }
    connect(actionButton, &BetterButton::signalButtonClicked, this, &ConfigWidget::showActionArea);
}

void ConfigWidget::showActionArea(const QString &actionName) {
    d->m_titleLabel->setText(actionName);
    d->m_stackedWidget->setCurrentWidget(d->m_actionMap->value(actionName));
}


} // namespace Tray::Ui
