//
// Created by cww on 25-4-10.
//
#pragma once

#include <QWidget>
class QLabel;
class Settings;
class QPushButton;
class QListWidget;


namespace UI::SettingsWidget {
    class SettingsWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit SettingsWidget(QWidget *parent = nullptr);

    private:
        QPushButton *m_addButton;
        QPushButton *m_removeButton;
        QListWidget *m_listWidget;
        QLabel *m_messageLabel;

    Q_SIGNALS:
        void signalAddButton(const QString &);

        void signalRemoveButton(const QString &);

    private Q_SLOTS:
        void addMusicPath();

        void removeMusicPath();

    public Q_SLOTS:
        void updateLocalPaths(const QStringList &paths);
    };
}
