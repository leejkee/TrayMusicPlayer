//
// Created by cww on 25-6-12.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui::Panel {

class VerticalButtonListWidgetPrivate;
class VerticalButtonListWidget final : public QWidget {

public:
    explicit VerticalButtonListWidget(const QStringList &list, QWidget *parent = nullptr);

private:
    std::unique_ptr<VerticalButtonListWidgetPrivate> d;
};
} // namespace Tray::Panel
