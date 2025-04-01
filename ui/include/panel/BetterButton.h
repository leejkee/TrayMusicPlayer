//
// Created by cww on 25-4-1.
//

#pragma once
#include <QPushButton>


namespace UI::Panel {
    class BetterButton final : public QPushButton {
        Q_OBJECT

    public:
        // others
        explicit BetterButton(QWidget *parent = nullptr);

        explicit BetterButton(const QIcon &icon, QWidget *parent = nullptr, const QString &name = "");

        // Music List buttons
        explicit BetterButton(const QString &name, QWidget *parent = nullptr);

        void loadStyleSheet(const QString &);

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

    Q_SIGNALS:
        void signalButtonClicked(const QString &listName);

    private Q_SLOTS:
        void onButtonClicked();

    private:
        void init();
    };
}
