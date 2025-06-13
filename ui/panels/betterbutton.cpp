//
// Created by cww on 25-4-1.
//
#include "betterbutton.h"
#include <uiconfig.h>
#include <QEvent>


namespace Tray::Ui::Panel {

    BetterButton::BetterButton(const BetterButtonMetaData &metaData, QWidget *parent) : QPushButton(parent){
        setIcon(QIcon(metaData.iconPath));
        setText(metaData.name);
        if (metaData.height > 0) {
            setFixedHeight(metaData.height);
        }
        if (metaData.width > 0) {
            setFixedWidth(metaData.width);
        }
        loadStyleSheet(metaData.qssPath);
        connect(this, &QPushButton::clicked, this, &BetterButton::onButtonClicked);
        installEventFilter(this);
    }

    BetterButton::BetterButton(QWidget *parent) : BetterButton({}, parent) {}


    void BetterButton::loadStyleSheet(const QString &qssPath) {
        if (qssPath.isEmpty()) {
            return;
        }
        this->setStyleSheet(readQSS(qssPath));
    }

    bool BetterButton::eventFilter(QObject *watched, QEvent *event) {
        if (watched == this) {
            if (event->type() == QEvent::Enter) {
                setCursor(Qt::PointingHandCursor);
            } else if (event->type() == QEvent::Leave) {
                setCursor(Qt::ArrowCursor);
            }
        }
        return QPushButton::eventFilter(watched, event);
    }

    void BetterButton::onButtonClicked() {
        Q_EMIT signalButtonClicked(this->text());
    }
}
