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
    inline const static auto PATH_ACTION_KEY = QStringLiteral("Local path");
    FilePathConfigWidget *m_filePathConfigWidget;
};

SettingsWidget::SettingsWidget(QWidget *parent)
    : ConfigWidget(parent), d(std::make_unique<SettingsWidgetPrivate>()) {
    d->m_filePathConfigWidget = new FilePathConfigWidget(this);
    addConfigWidget({SettingsWidgetPrivate::PATH_ACTION_KEY, Res::DirectoryActionSVG, Res::CONFIG_ACTION_QSS, 30, 100},
                    d->m_filePathConfigWidget);
}

SettingsWidget::~SettingsWidget() = default;

void SettingsWidget::updateLocalPaths(const QStringList &localDir) {
    d->m_filePathConfigWidget->updateLocalPaths(localDir);
}
}
