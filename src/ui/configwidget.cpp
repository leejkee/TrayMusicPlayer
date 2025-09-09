#include <configwidget/configwidget.h>
#include <stylebutton/stylebutton.h>
#include <log/log.h>
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
};

ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent), d(std::make_unique<ConfigWidgetPrivate>()){
    d->m_actionMap = std::make_unique<QHash<QString, QWidget*>>();
    d->m_stackedWidget = new QStackedWidget(this);
    d->m_titleLabel = new QLabel(this);
    d->m_buttonLayout = new QVBoxLayout;
    d->m_buttonLayout->setSpacing(0);
    d->m_areaLayout = new QVBoxLayout;
    d->m_mainLayout = new QHBoxLayout;

    d->m_areaLayout->addWidget(d->m_titleLabel);
    d->m_areaLayout->addWidget(d->m_stackedWidget);

    d->m_mainLayout->addLayout(d->m_buttonLayout);
    d->m_mainLayout->addLayout(d->m_areaLayout);
    setLayout(d->m_mainLayout);
}


void ConfigWidget::addConfigWidget(const QString &actionName, QWidget *w) {
    addConfigWidget(actionName, {}, {}, {}, w);
}


void ConfigWidget::addConfigWidget(const QString& buttonId, const QSize& size
                , const QString& iconPath
                , const QString& qssPath
                ,  QWidget *w) {
    if (w == nullptr) {
        LOG_ERROR("Null widget, do nothing");
        return;
    }
    auto* actionButton = new StyleButton(buttonId, size, iconPath, qssPath, this);

    if (d->m_buttonLayout->count() == 0) {
        d->m_buttonLayout->addWidget(actionButton);
        d->m_buttonLayout->addStretch();
    }
    else {
        d->m_buttonLayout->insertWidget(d->m_buttonLayout->count() - 1, actionButton);
    }
    d->m_actionMap->insert(buttonId, w);
    d->m_stackedWidget->addWidget(w);

    if (d->m_stackedWidget->count() == 1) {
        showActionArea(buttonId);
    }
    connect(actionButton, &StyleButton::signalButtonClicked, this, &ConfigWidget::showActionArea);
}

void ConfigWidget::showActionArea(const QString &actionName) {
    d->m_titleLabel->setText(actionName);
    d->m_stackedWidget->setCurrentWidget(d->m_actionMap->value(actionName));
}


} // namespace Tray::Ui
