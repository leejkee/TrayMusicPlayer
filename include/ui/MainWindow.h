//
// Created by cww on 25-2-13.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class QSystemTrayIcon;
class WindowManager;
class Settings;

class MainWindow final : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;
    void setVisible(bool visible) override;


private:
    void initMainApplication();
    void createConnections();
    void createTrayIcon();
    void loadSettings();

    QAction* m_minimizeAction;
    QAction* m_maximizeAction;
    QAction* m_restoreAction;
    QAction* m_quitAction;
    QSystemTrayIcon *m_systemTrayIcon;
    QMenu *m_trayIconMenu;
    //
    // WindowManager *m_windowManager;
};


#endif //MAINWINDOW_H
