//
// Created by cww on 25-6-15.
//
#pragma once
#include <memory>
#include <configwidget/configwidget.h>

namespace Tray::Ui {

class SettingsWidgetPrivate;
class SettingsWidget final : public Panel::ConfigWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget() override;

public Q_SLOTS:
    void updateLocalPaths(const QStringList &localDir);

    Q_SIGNALS:
    void signalLocalDirAdded(const QString &);
    void signalLocalDirRemoved(const QString &);

private:
    std::unique_ptr<SettingsWidgetPrivate> d;
};
}