//
// Created by 31305 on 25-6-17.
//
#include <QApplication>
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget w;
    QLabel* label_up = new QLabel("Up");
    QLabel* label_down = new QLabel("Down");
    QLabel* label_other = new QLabel("Other");
    QLabel* label_other1 = new QLabel("Other1");
    label_down->setFixedHeight(20);
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    QVBoxLayout* layout = new QVBoxLayout;
    splitter->addWidget(label_up);
    splitter->addWidget(label_down);
    splitter->addWidget(label_other);
    splitter->addWidget(label_other1);
    splitter->setStyleSheet(
        "QSplitter::handle {"
        "    background-color: gray;" // Color of the handle
        "}"
        "QSplitter::handle:vertical {"
        "    height: 1px;" // Height of the vertical handle
        "}"
        "QSplitter::handle:horizontal {"
        "    width: 1px;" // Width of the horizontal handle (not relevant here but good to know)
        "}"
    );
    layout->setContentsMargins(0, 10, 0, 10);
    layout->addWidget(splitter);
    w.setLayout(layout);
    w.show();
    return QApplication::exec();
}
