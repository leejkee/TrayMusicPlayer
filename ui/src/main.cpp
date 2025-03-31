#include <mainwindow/MainWindow.h>
#include <QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        const auto choice = QMessageBox::critical(nullptr, QObject::tr("Systray"),
            QObject::tr("I couldn't detect any system tray on this system."),
            QMessageBox::Close | QMessageBox::Ignore);
        if (choice == QMessageBox::Close)
            return 1;
        // Otherwise "lurk": if a system tray is started later, the icon will appear.
    }
    // QApplication::setQuitOnLastWindowClosed(false);

    Mainwindow::MainWindow w;
    w.show();


    return QApplication::exec();
}

