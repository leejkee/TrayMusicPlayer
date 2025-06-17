//
// Created by cww on 25-6-14.
//
#include <configwidget.h>
#include <QApplication>
#include <QLabel>


using namespace Tray::Ui;
int main(int argc, char** argv)
{
    Q_INIT_RESOURCE(qss);
    Q_INIT_RESOURCE(svg);
    QApplication app(argc, argv);
    const auto configWidget = std::make_unique<Panel::ConfigWidget>();
    auto label_1 = new QLabel("Label 1");
    auto label_2 = new QLabel("Label 2");
    configWidget->addConfigWidget("test 1", label_1);
    configWidget->addConfigWidget("test 2", label_2);
    configWidget->show();
    return QApplication::exec();
}
