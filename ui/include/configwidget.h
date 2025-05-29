#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui {

class ConfigWidgetPrivate;
class ConfigWidget final : public QWidget {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);

private:
    std::unique_ptr<ConfigWidgetPrivate> d;

};
} // namespace Tray::Ui