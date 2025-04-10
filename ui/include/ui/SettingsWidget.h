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

        void loadSettings();

    private:
        QPushButton *m_addButton;
        QPushButton *m_removeButton;
        QListWidget *m_listWidget;
        QLabel *m_messageLabel;

    Q_SIGNALS:
        // signals for settings changed
        // 1, send to the settings
        // 2, send to the  viewWidget to refresh if the shown widget is "local"
        void signalLocalMusicPathSettingsChanged();

        void signalAddButton(const QString &);

        void signalRemoveButton(const QString &);

    private Q_SLOTS:
        void addMusicPath();

        void removeMusicPath();
    };
}
