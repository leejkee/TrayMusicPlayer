//
// Created by cww on 25-3-31.
//
#pragma once
#include "IPlayer.h"
#include <QMediaPlayer>

namespace Core::Player{

class Player : public IPlayer {

public:
    Player();

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_output;
};
}