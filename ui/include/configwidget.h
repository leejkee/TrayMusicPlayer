#pragma once
#include <uimetadata.h>
#include <QDialog>
#include <memory>

namespace Tray::Ui::Panel {

class ConfigWidgetPrivate;
class ConfigWidget : public QWidget {
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