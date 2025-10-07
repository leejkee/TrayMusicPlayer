#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include "trayapp.h"

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        const auto choice = QMessageBox::critical(nullptr
                                                  , QObject::tr("Settings file selection")
                                                  , QObject::tr("Error: Too many arguments provided.")
                                                  , QMessageBox::Close |
                                                  QMessageBox::Ignore);
        if (choice == QMessageBox::Close)
        {
            return 1;
        }
    }

    QApplication app(argc, argv);
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        const auto choice = QMessageBox::critical(nullptr
                                                  , QObject::tr("Systray")
                                                  , QObject::tr("I couldn't detect any system tray on this system.")
                                                  , QMessageBox::Close |
                                                  QMessageBox::Ignore);
        if (choice == QMessageBox::Close)
        {
            return 1;
        }
        // Otherwise "lurk": if a system tray is started later, the icon will appear.
    }

    QString cmdline;
    if (argc == 2)
    {
        cmdline = QString(argv[1]);
    }
    Tray::TrayApp trayapp(cmdline);
    trayapp.show();
    QApplication::setQuitOnLastWindowClosed(false);

    return QApplication::exec();
}
