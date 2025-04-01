//
// Created by cww on 25-4-1.
//
#include <ui/BetterButton.h>
#include <ui/Assets.h>
#include <QEvent>

namespace UI::BetterButton {
    BetterButton::BetterButton(const QString &name, QWidget *parent) : QPushButton(parent) {
        setIcon(QIcon(SvgRes::MusicListSVG));
        // setFixedHeight(30);
        setText(name);
        m_listName = name;
        loadStyleSheet(QssRes::BUTTON_LIST_QSS);
        connect(this, &QPushButton::clicked, this , &BetterButton::onButtonClicked);
        installEventFilter(this);
    }

    BetterButton::BetterButton(QWidget *parent) : QPushButton(parent) {
        installEventFilter(this);
    }

    BetterButton::BetterButton(const QIcon &icon, QWidget *parent, QString name) : QPushButton(parent)
            , m_listName(std::move(name)){
        setIcon(icon);
        setText(m_listName);
        loadStyleSheet(QssRes::BUTTON_LIST_QSS);
        installEventFilter(this);
    }


    void BetterButton::loadStyleSheet(const QString &qssPath) {
        this->setStyleSheet(Tools::readQSS(qssPath));
    }

    bool BetterButton::eventFilter(QObject *watched, QEvent *event) {
        if (watched == this) {
            if (event->type() == QEvent::Enter) {
                setCursor(Qt::PointingHandCursor);
            }
            else if (event->type() == QEvent::Leave) {
                setCursor(Qt::ArrowCursor);
            }
        }
        return QPushButton::eventFilter(watched, event);
    }
}