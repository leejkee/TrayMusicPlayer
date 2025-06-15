//
// Created by cww on 25-4-1.
//

#pragma once
#include <../include/uimetadata.h>
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

    explicit BetterButton(QWidget *parent = nullptr);

    explicit BetterButton(const BetterButtonMetaData &metaData, QWidget *parent = nullptr);


    void loadStyleSheet(const QString &);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    void signalButtonClicked(const QString &listName);

private Q_SLOTS:
    void onButtonClicked();
};
}
