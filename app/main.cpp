#include "TrayApp.h"
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
    QApplication::setQuitOnLastWindowClosed(false);

    std::unique_ptr<Tray::TrayApp> w;

    if (argc < 2) {
        w = std::make_unique<Tray::TrayApp>();
    }
    else {
        w = std::make_unique<Tray::TrayApp>(argv[1]);
    }

    w->show();
    return QApplication::exec();
}

