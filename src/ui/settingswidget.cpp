//
// Created by cww on 25-4-10.
//
#include "filepathconfigwidget.h"
#include <settingswidget.h>
#include <trayqss.h>
#include <traysvg.h>


namespace Tray::Ui {
class SettingsWidgetPrivate {
public:
    constexpr static int BTN_HEIGHT = 30;
    constexpr static int BTN_WIDTH = 90;
    inline const static auto PATH_ACTION_KEY = QStringLiteral("Local path");

    FilePathConfigWidget *m_filePathConfigWidget;
};

SettingsWidget::SettingsWidget(QWidget *parent)
    : ConfigWidget(parent), d(std::make_unique<SettingsWidgetPrivate>()) {
    d->m_filePathConfigWidget = new FilePathConfigWidget(this);
    addConfigWidget({
                        SettingsWidgetPrivate::PATH_ACTION_KEY, Res::DirectoryActionSVG, Res::CONFIG_ACTION_QSS,
                        SettingsWidgetPrivate::BTN_HEIGHT, SettingsWidgetPrivate::BTN_WIDTH
                    }
                    , d->m_filePathConfigWidget);

    connect(d->m_filePathConfigWidget, &FilePathConfigWidget::signalLocalDirAdded,
            this, [this](const QString &d) {
                Q_EMIT signalLocalDirAdded(d);
            });
    connect(d->m_filePathConfigWidget, &FilePathConfigWidget::signalLocalDirRemoved,
            this, [this](const QString &d) {
                Q_EMIT signalLocalDirRemoved(d);
            });
}

SettingsWidget::~SettingsWidget() = default;

void SettingsWidget::updateLocalPaths(const QStringList &localDir) {
    d->m_filePathConfigWidget->updateLocalPaths(localDir);
}
}
