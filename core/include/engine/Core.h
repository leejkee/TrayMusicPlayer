#pragma once
#include "ICore.h"
#include "Player.h"

namespace Core::Engine {
class Core : public ICore {
public:
    Core(QObject* parent = nullptr);

private:
    Player::Player* m_player;
};
} // namespace Core