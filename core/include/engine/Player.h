//
// Created by cww on 25-3-31.
//
#pragma once
#include <QMediaPlayer>
#include <QObject>

namespace Core::Engine {

class Player : public QObject{

public:
    explicit Player(QObject *parent = nullptr);

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_output;
};
}