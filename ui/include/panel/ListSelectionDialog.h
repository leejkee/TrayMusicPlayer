//
// Created by cww on 25-4-26.
//
#pragma once

#include <QDialog>
#include <QStringList>

class QListWidget;
class QLabel;

namespace UI::Panel {
    class ListSelectionDialog final: public QDialog {
        Q_OBJECT

    public:
        explicit ListSelectionDialog(const QStringList &items, QWidget *parent = nullptr);

        QString selectedItem() const;

    private:
        QLabel *m_title;
        QListWidget *m_listWidget;
        QString m_selectedItem;
    };
}
