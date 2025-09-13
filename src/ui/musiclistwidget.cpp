//
// Created by cww on 25-4-10.
//
#include <musiclistwidget/musiclistwidget.h>
#include <stylebutton/stylebutton.h>
#include <log/log.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QInputDialog>
#include <QMenu>

namespace Tray::Ui
{
class MusicListWidgetPrivate
{
public:
    Panel::StyleButton* m_expandButton;
    Panel::StyleButton* m_addButton;
    QScrollArea* m_scrollArea;
    QWidget* m_buttonWidget;
    QVBoxLayout* m_buttonLayout;
    Panel::StyleButton* m_localListButton;
    QHash<QString, Panel::StyleButton*> m_UserButtonHash;

    static inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
    static inline const auto EXPAND_BTN_TEXT = QStringLiteral("User's");
    static inline const auto DEL_TEXT = QStringLiteral("Delete");
};

MusicListWidget::MusicListWidget(QWidget* parent)
    : QWidget(parent), d(std::make_unique<MusicListWidgetPrivate>())
{
    d->m_localListButton = new
            Panel::StyleButton(MusicListWidgetPrivate::LOCAL_LIST_KEY
                               , {30, 0}
                               , Res::MusicListSVG
                               , Res::BUTTON_NORMAL_QSS
                               , this);
    d->m_expandButton = new
            Panel::StyleButton(MusicListWidgetPrivate::EXPAND_BTN_TEXT
                               , {30, 60}
                               , Res::UpSVG
                               , Res::BUTTON_EXPAND_QSS
                               , this);
    d->m_addButton = new Panel::StyleButton({}
                                            , {}
                                            , Res::AddSVG
                                            , Res::BUTTON_ADD_QSS
                                            , this);
    const auto buttonLayout = new QHBoxLayout;
    const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);
    buttonLayout->addWidget(d->m_expandButton);
    buttonLayout->addItem(spaceH);
    buttonLayout->addWidget(d->m_addButton);
    d->m_buttonWidget = new QWidget(this);
    d->m_buttonLayout = new QVBoxLayout;
    d->m_buttonLayout->setSpacing(0);
    d->m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    d->m_buttonWidget->setLayout(d->m_buttonLayout);

    d->m_scrollArea = new QScrollArea(this);
    d->m_scrollArea->setWidgetResizable(true);
    d->m_scrollArea->setFrameShape(QFrame::NoFrame);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QSpacerItem* spaceV = new QSpacerItem(0
                                        , -1
                                        , QSizePolicy::Minimum
                                        , QSizePolicy::Expanding);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(d->m_localListButton);
    mainLayout->addItem(buttonLayout);
    mainLayout->addWidget(d->m_buttonWidget);
    mainLayout->addItem(spaceV);

    const auto mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    d->m_scrollArea->setWidget(mainWidget);
    // d->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    const auto layout = new QVBoxLayout;
    layout->addWidget(d->m_scrollArea);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    createConnections();
}

MusicListWidget::~MusicListWidget() = default;

void MusicListWidget::createConnections()
{
    connect(d->m_localListButton
            , &Panel::StyleButton::signalButtonClicked
            , this
            , &MusicListWidget::handleMusicButtonClicked);
    connect(d->m_expandButton
            , &QPushButton::clicked
            , this
            , &MusicListWidget::toggleExpand);
    connect(d->m_addButton
            , &QPushButton::clicked
            , this
            , &MusicListWidget::newUserPlaylist);
}


void MusicListWidget::toggleExpand() const
{
    const bool isV = d->m_buttonWidget->isVisible();
    d->m_buttonWidget->setVisible(!isV);
    if (!isV)
    {
        d->m_expandButton->setIcon(QIcon(Res::UpSVG));
    }
    else
    {
        d->m_expandButton->setIcon(QIcon(Res::DownSVG));
    }
}

void MusicListWidget::newUserPlaylist()
{
    bool ok;
    const QString playlistName = QInputDialog::getText(this
        , "New a music list"
        , "Music list name:"
        , QLineEdit::Normal
        , ""
        , &ok);
    if (ok && !playlistName.isEmpty() && !playlistName.begin()->isDigit())
    {
        LOG_INFO("MusicListWidget: [" + playlistName + "] added.");
        Q_EMIT signalUserPlaylistButtonAdded(playlistName);
    }
}

void MusicListWidget::appendUserButton(const QString& playlistName)
{
    auto* button = new Panel::StyleButton(playlistName
                                          , {30, 0}
                                          , Res::MusicListSVG
                                          , Res::BUTTON_NORMAL_QSS
                                          , this);
    d->m_UserButtonHash[playlistName] = button;
    d->m_buttonLayout->addWidget(button);
    button->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(button
            , &Panel::StyleButton::signalButtonClicked
            , this
            , &MusicListWidget::handleMusicButtonClicked);
    connect(button
            , &Panel::StyleButton::customContextMenuRequested
            , this
            , &MusicListWidget::handleContextMenu);
}

void MusicListWidget::handleContextMenu(const QPoint& pos)
{
    auto* button = qobject_cast<Panel::StyleButton*>(sender());
    auto* listMenu = new QMenu(this);
    connect(listMenu, &QMenu::aboutToHide, listMenu, &QMenu::deleteLater);
    const QAction* deleteAction = listMenu->addAction(MusicListWidgetPrivate::DEL_TEXT);
    connect(deleteAction
            , &QAction::triggered
            , this
            , [this, button]
            {
                Q_EMIT signalUserPlaylistButtonRemoved(button->text());
            });
    listMenu->exec(button->mapToGlobal(pos));
}

void MusicListWidget::removeUserButton(const QString& playlistName)
{
    if (QPushButton* buttonToRemove = d->m_UserButtonHash.take(playlistName))
    {
        d->m_buttonLayout->removeWidget(buttonToRemove);
        delete buttonToRemove;
    }
}

void MusicListWidget::initUserListButtons(const QStringList& playlistNames)
{
    for (const auto& name : playlistNames)
    {
        appendUserButton(name);
    }
}

void MusicListWidget::handleMusicButtonClicked(const QString& name)
{
    LOG_INFO("buttonWidget handleMusicButtonClicked");
    Q_EMIT signalMusicListButtonClicked(name);
}
}
