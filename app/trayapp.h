//
// Created by cww on 25-2-13.
//
#pragma once
#include <QMainWindow>
#include <memory>


namespace Tray {

    class TrayAppPrivate;
    class TrayApp final : public QMainWindow {
        Q_OBJECT

    public:

        explicit TrayApp(QWidget *parent = nullptr);

        ~TrayApp() override;

    protected:
        void closeEvent(QCloseEvent *event) override;

        void setVisible(bool visible) override;

        // void paintEvent(QPaintEvent *event) override;
        
    private:
        std::unique_ptr<TrayAppPrivate> d;

        void initConnections();
    };
}
