#pragma once
#include <QDialog>
#include <memory>

namespace Tray::Ui {

class ConfigWidgetPrivate;
class ConfigWidget final : public QDialog {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);

private:
    std::unique_ptr<ConfigWidgetPrivate> d;
    

    /// Initialize widget layout and signal/slot connections
    /// 1. create the layout for the widget
    /// 2. initialize the areas of the settings
    /// 3. connect the signals and slots 
    void initConfigration();

};
} // namespace Tray::Ui