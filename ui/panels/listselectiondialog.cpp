//
// Created by cww on 25-4-26.
//
#include "listselectiondialog.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>


namespace Tray::Ui::Panel {
    ListSelectionDialog::ListSelectionDialog(const QStringList &items, QWidget *parent)
        : QDialog(parent) {
        setWindowTitle(tr("Music List Selection"));
        m_listWidget = new QListWidget(this);
        m_title = new QLabel(this);
        m_title->setText("选择歌单");
        m_title->setAlignment(Qt::AlignCenter);
        m_listWidget->addItems(items);

        auto *layout = new QVBoxLayout(this);
        layout->addWidget(m_title);
        layout->addWidget(m_listWidget);

        connect(m_listWidget, &QListWidget::itemDoubleClicked, this, [this](const QListWidgetItem *item) {
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
