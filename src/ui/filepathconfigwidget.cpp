#include <settingswidget/filepathconfigwidget.h>
#include <stylebutton/stylebutton.h>
#include <trayqss.h>
#include <traysvg.h>
#include <QCoreApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSpacerItem>

namespace Tray::Ui
{
class FilepathConfigWidgetPrivate
{
public:
    Panel::StyleButton* m_addButton;
    Panel::StyleButton* m_removeButton;
    QListWidget* m_listWidget;
    QLabel* m_messageLabel;

    static inline int BUTTON_SIZE = 30;
    static inline auto MESSAGE_TEXT = QStringLiteral("Local Music Paths: ");
};

FilePathConfigWidget::FilePathConfigWidget(QWidget* parent)
    : QWidget(parent)
{
    d->m_addButton = new Panel::StyleButton({}
                                            , {
                                                FilepathConfigWidgetPrivate::BUTTON_SIZE
                                                , FilepathConfigWidgetPrivate::BUTTON_SIZE
                                            }
                                            , Res::AddSVG
                                            , Res::BUTTON_SQUARE_QSS
                                            , this);
    d->m_listWidget = new QListWidget(this);
    d->m_removeButton = new Panel::StyleButton({}
                                               , {
                                                   FilepathConfigWidgetPrivate::BUTTON_SIZE
                                                   , FilepathConfigWidgetPrivate::BUTTON_SIZE
                                               }
                                               , Res::RemoveSVG
                                               , Res::BUTTON_SQUARE_QSS
                                               , this);
    d->m_messageLabel = new QLabel(this);
    d->m_messageLabel->setText(FilepathConfigWidgetPrivate::MESSAGE_TEXT);
    d->m_listWidget->setStyleSheet("QListWidget { font-size: 15px; }");

    const auto hlayout = new QHBoxLayout;
    const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);

    hlayout->addWidget(d->m_messageLabel);
    hlayout->addItem(spaceH);
    hlayout->addWidget(d->m_addButton);
    hlayout->addWidget(d->m_removeButton);

    const auto layout = new QVBoxLayout(this);
    layout->addItem(hlayout);
    layout->addWidget(d->m_listWidget);

    connect(d->m_addButton
            , &QPushButton::clicked
            , this
            , &FilePathConfigWidget::addMusicPath);
    connect(d->m_removeButton
            , &QPushButton::clicked
            , this
            , &FilePathConfigWidget::removeMusicPath);
}

FilePathConfigWidget::~FilePathConfigWidget() = default;

void FilePathConfigWidget::addMusicPath()
{
    const QString newMusicPath = QFileDialog::getExistingDirectory(
     this
     , tr("Open Directory")
     , QCoreApplication::applicationDirPath()
     , QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    Q_EMIT signalLocalDirAdded(newMusicPath);
}

void FilePathConfigWidget::removeMusicPath()
{
    Q_EMIT signalLocalDirRemoved(d->m_listWidget->currentItem()->text());
}

void FilePathConfigWidget::updateLocalPaths(const QStringList& paths)
{
    d->m_listWidget->clear();
    d->m_listWidget->addItems(paths);
}
} // namespace Tray::Ui
