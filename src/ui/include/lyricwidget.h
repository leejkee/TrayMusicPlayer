//
// Created by cww on 25-6-15.
//
#pragma once
#include <QWidget>

namespace Tray::Ui
{
class LyricWidget : public QWidget
{
public:
    explicit LyricWidget(QWidget* parent = nullptr);
    ~LyricWidget() override;

    enum LyricShowMode
    {
    };
};
} // namespace Tray::Ui
