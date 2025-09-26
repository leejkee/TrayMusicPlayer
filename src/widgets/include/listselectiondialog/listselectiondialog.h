//
// Created by cww on 25-4-26.
//
#pragma once

#include <QDialog>

class QListWidget;
class QLabel;

namespace Tray::Ui::Panel {
    class ListSelectionDialog final: public QDialog {
        Q_OBJECT

    public:
        explicit ListSelectionDialog(const QStringList &items, QWidget *parent = nullptr);

        [[nodiscard]] QString selectedItem() const;

    private:
        QLabel *m_title;
        QListWidget *m_listWidget;
        QString m_selectedItem;
    };
}
