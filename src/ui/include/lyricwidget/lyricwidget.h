//
// Created by 31305 on 2025/7/16.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui
{

class LyricWidgetPrivate;
class LyricWidget final : public QWidget
{
    Q_OBJECT
    public:
    explicit LyricWidget(QWidget *parent = nullptr);
    ~LyricWidget() override;
public slots:

private:
    std::unique_ptr<LyricWidgetPrivate> d;
};
}
