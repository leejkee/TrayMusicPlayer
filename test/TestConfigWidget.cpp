//
// Created by cww on 25-6-14.
//
#include <filepathconfigwidget.h>
#include <configwidget.h>
#include <QApplication>
#include <memory>


using namespace Tray::Ui;
int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(qss);
    Q_INIT_RESOURCE(svg);
    QApplication app(argc, argv);
    const auto configWidget = std::make_unique<ConfigWidget>();
    auto *w = new FilePathConfigWidget;
    QWidget *blankWidget = new QWidget;
    configWidget->addConfigWidget("null", blankWidget);
    configWidget->addConfigWidget("test", w);
    configWidget->show();
    return QApplication::exec();
}
