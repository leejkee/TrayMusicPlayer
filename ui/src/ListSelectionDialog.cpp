//
// Created by cww on 25-4-26.
//
#include <panel/ListSelectionDialog.h>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>


namespace UI::Panel {
    ListSelectionDialog::ListSelectionDialog(const QStringList &items, QWidget *parent)
        : QDialog(parent) {
        setWindowTitle("选择项目");
        resize(300, 400);

        m_listWidget = new QListWidget(this);
        m_listWidget->addItems(items);

        auto *confirmButton = new QPushButton("确定", this);

        auto *layout = new QVBoxLayout(this);
        layout->addWidget(m_listWidget);
        layout->addWidget(confirmButton);

        connect(confirmButton, &QPushButton::clicked, this, [this]() {
            if (m_listWidget->currentItem()) {
                m_selectedItem = m_listWidget->currentItem()->text();
                accept();
            }
        });

        connect(m_listWidget, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem *item) {
            if (item) {
                m_selectedItem = item->text();
                accept();
            }
        });
    }

    QString ListSelectionDialog::selectedItem() const {
        return m_selectedItem;
    }
}
