//
// Created by cww on 25-6-12.
//

#include "verticalbuttonlistwidget.h"
#include <betterbutton.h>


namespace Tray::Ui::Panel {
class VerticalButtonListWidgetPrivate {
public:
    QHash<QString, BetterButton* > m_buttonMap;
};

VerticalButtonListWidget::VerticalButtonListWidget(const QStringList&list, QWidget *parent)
    : QWidget(parent),
      d(std::make_unique<VerticalButtonListWidgetPrivate>()) {
    for (const auto& action: list) {
        d->m_buttonMap[action] = new BetterButton(action, this);
    }
}
} // Tray
