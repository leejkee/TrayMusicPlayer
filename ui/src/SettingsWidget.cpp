//
// Created by cww on 25-4-10.
//

#include <ui/SettingsWidget.h>
#include <ui/Assets.h>

#include <QCoreApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QSpacerItem>


namespace UI::SettingsWidget {
    SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent) {
        this->m_addButton = new QPushButton(QIcon(SvgRes::AddSVG), "", this);
        m_addButton->setStyleSheet(Tools::readQSS(QssRes::BUTTON_SQUARE_QSS));
        this->m_listWidget = new QListWidget(this);
        this->m_removeButton = new QPushButton(QIcon(SvgRes::RemoveSVG), "", this);
        m_removeButton->setStyleSheet(Tools::readQSS(QssRes::BUTTON_SQUARE_QSS));
        this->m_messageLabel = new QLabel(this);
        m_messageLabel->setText("Local Music Paths: ");
        m_listWidget->setStyleSheet("QListWidget { font-size: 15px; }");

        const auto hlayout = new QHBoxLayout;
        const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);
        hlayout->addWidget(m_messageLabel);
        hlayout->addItem(spaceH);
        hlayout->addWidget(m_addButton);
        hlayout->addWidget(m_removeButton);

        const auto layout = new QVBoxLayout(this);
        layout->addItem(hlayout);
        layout->addWidget(m_listWidget);
        connect(m_addButton, &QPushButton::clicked, this, &SettingsWidget::addMusicPath);
        connect(m_removeButton, &QPushButton::clicked, this, &SettingsWidget::removeMusicPath);
    }

    void SettingsWidget::addMusicPath() {
        const QString newMusicPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                       QCoreApplication::applicationDirPath(),
                                                                       QFileDialog::ShowDirsOnly
                                                                       | QFileDialog::DontResolveSymlinks);
        if (!newMusicPath.isEmpty()) {
            Q_EMIT signalAddButton(newMusicPath);
        }
    }


    void SettingsWidget::removeMusicPath() {
        const QListWidgetItem *item = this->m_listWidget->currentItem();
        if (!item) {
            qDebug() << "No music path selected";
            return;
        }
        const auto path = item->text();
        Q_EMIT signalRemoveButton(path);
    }

    void SettingsWidget::updateLocalPaths(const QStringList &paths) {
        m_listWidget->clear();
        m_listWidget->addItems(paths);
    }
}
