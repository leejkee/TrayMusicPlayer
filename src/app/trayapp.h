//
// Created by cww on 25-2-13.
//
#pragma once
#include "stylewindow/stylewindow.h"
#include <memory>


namespace Tray
{
class TrayAppPrivate;

class TrayApp final : public Ui::StyleWindow
{
    Q_OBJECT
public:
    explicit TrayApp(QWidget* parent = nullptr);
    explicit TrayApp(const QString& configPath, QWidget* parent = nullptr);

    ~TrayApp() override;

protected:
    void closeEvent(QCloseEvent* event) override;

    void setVisible(bool visible) override;

    // void paintEvent(QPaintEvent *event) override;

private:
    std::unique_ptr<TrayAppPrivate> d;

    void connectCoreWindow();

    void initAppIcons();
};
}
