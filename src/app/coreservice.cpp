//
// Created by 31305 on 2025/8/26.
//
#include "coreservice.h"
#include <player/player.h>
#include <playlist/playlist.h>
#include <listcache/listcache.h>
#include <settings/settings.h>
#include <lyricservice/lyricservice.h>

#include <QThread>
namespace Tray::App
{
class CoreServicePrivate
{
public:
    Core::Player *m_player;
    Core::PlayList *m_playlist;
    Core::Settings *m_settings;

    Core::ListCache *m_listCache;
    Core::LyricService *m_lyricService;
    QThread* m_listCacheThread;
    QThread* m_lyricServiceThread;
};

CoreService::CoreService(QObject *parent) : QObject(parent), d(std::make_unique<CoreServicePrivate>())
{
    d->m_settings = new Core::Settings(this);
    d->m_player = new Core::Player(this);
    d->m_playlist = new Core::PlayList(this);
    d->m_lyricService = new Core::LyricService(this);
    d->m_listCache = new Core::ListCache(this);
    initConnections();
    initPreload();
}

void CoreService::initConnections()
{
}

/// init function
/// 1) settings init
/// 2) listCache loaded
/// 3) playlist loaded
/// 4) player loaded
///

void CoreService::initPreload()
{
    d->m_listCache->init(d->m_settings->getLocalMusicDirectories(), d->m_settings->getKeysUserPlaylist());
    d->m_playlist->loadMusicList()
}



CoreService::~CoreService() = default;


}
