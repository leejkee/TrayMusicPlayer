//
// Created by 31305 on 2025/8/26.
//
#pragma once
#include <memory>
#include <QObject>

namespace Tray::App
{
class CoreServicePrivate;
class CoreService final: public QObject
{
public:
    explicit CoreService(QObject *parent = nullptr);

    void initConnections();

    void initPreload();

    ~CoreService() override;

private:
    std::unique_ptr<CoreServicePrivate> d;
};

}
