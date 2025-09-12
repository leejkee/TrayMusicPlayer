//
// Created by cww on 25-4-10.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui
{
class TopBarWidgetPrivate;
class TopBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TopBarWidget(QWidget* parent = nullptr);

    ~TopBarWidget() override;

    void setPreBtnShow(bool b);

Q_SIGNALS:
    void signalPreButtonClicked();

    void signalSettingsButtonClicked();

private:
    std::unique_ptr<TopBarWidgetPrivate> d;
};
}
