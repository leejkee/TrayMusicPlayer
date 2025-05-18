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
        static constexpr int MAIN_MINIMUM_WIDTH = 600;
        static constexpr int MAIN_MINIMUM_HEIGHT = 450;

        explicit TrayApp(QWidget *parent = nullptr);

        ~TrayApp() override;

    protected:
        void closeEvent(QCloseEvent *event) override;

        void setVisible(bool visible) override;

    private:

        void createConnections();

        void createTrayIcon();

        std::unique_ptr<TrayAppPrivate> d;

    };
}
