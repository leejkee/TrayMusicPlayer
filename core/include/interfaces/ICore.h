//
// Created by cww on 25-4-2.
//
#pragma once
#include <QObject>
#include <QString>

namespace Core {
class ICore : public QObject {
    Q_OBJECT
public:
    ICore(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual void playToggle() = 0;
    virtual void nextMusic() = 0;
    virtual void preMusic() = 0;
    virtual void setVolume(float) = 0;
    virtual void loadMusic(QString) = 0;

};
} // namespace Core
