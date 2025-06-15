//
// Created by cww on 25-6-15.
//
#pragma once
#include <configwidget.h>
#include <memory>

namespace Tray::Ui {

class SettingsWidgetPrivate;
class SettingsWidget final : public Panel::ConfigWidget {
public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget() override;

    void updateLocalPaths(const QStringList &localDir);

private:
    std::unique_ptr<SettingsWidgetPrivate> d;
};
}