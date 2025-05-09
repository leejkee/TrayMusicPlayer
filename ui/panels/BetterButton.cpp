//
// Created by cww on 25-4-1.
//
#include "BetterButton.h"
#include <Assets.h>
#include <QEvent>


namespace Tray::Ui::Panel {
    void BetterButton::init() {
        installEventFilter(this);
    }

    BetterButton::BetterButton(const QString &name, QWidget *parent) : QPushButton(parent) {
        setIcon(QIcon(SvgRes::MusicListSVG));
        setText(name);
        loadStyleSheet(QssRes::BUTTON_LIST_QSS);
        connect(this, &QPushButton::clicked, this, &BetterButton::onButtonClicked);
        init();
    }

    BetterButton::BetterButton(QWidget *parent) : QPushButton(parent) {
        init();
    }

    BetterButton::BetterButton(const QString &name, const QIcon &icon, QWidget *parent) : QPushButton(parent) {
        setIcon(icon);
        setText(name);
        loadStyleSheet(QssRes::BUTTON_LIST_QSS);
        init();
    }
    BetterButton::BetterButton(const QIcon &icon, QWidget *parent, const StyleMode style, const QString &name)
        : QPushButton(parent) {
        setIcon(icon);
        setText(name);
        setFixedSize(DefaultWidth, DefaultHeight);
        if (style == WithQss) {
            loadStyleSheet(QssRes::BUTTON_LIST_QSS);
        }
        init();
    }

    void BetterButton::loadStyleSheet(const QString &qssPath) {
        this->setStyleSheet(Tools::readQSS(qssPath));
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
