#pragma once
#include <QDialog>
#include <memory>
#include <uimetadata.h>

namespace Tray::Ui {

class ConfigWidgetPrivate;
class ConfigWidget final : public QWidget {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);

    void addConfigWidget(const QString& actionName, QWidget* w);
    void addConfigWidget(const BetterButtonMetaData &actionInfo, QWidget* w);
    ~ConfigWidget() override;

private:
    std::unique_ptr<ConfigWidgetPrivate> d;
    

private Q_SLOTS:
    void showActionArea(const QString &actionName);

};
} // namespace Tray::Ui