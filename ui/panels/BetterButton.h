//
// Created by cww on 25-4-1.
//

#pragma once
#include <QPushButton>


namespace Tray::Ui::Panel {
    class BetterButton final : public QPushButton {
        Q_OBJECT

    public:
        enum StyleMode {
            WithQss,
            NoQss
        };
        constexpr static int DefaultWidth = 30;
        constexpr static int DefaultHeight = 30;
        // others
        explicit BetterButton(QWidget *parent = nullptr);

        BetterButton(const QString &name, const QIcon &icon, QWidget *parent = nullptr);

        explicit BetterButton(const QIcon &icon, QWidget *parent = nullptr, StyleMode = WithQss, const QString &name = "");

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
