#pragma once

#include "enemy.hpp"


/// Дрон-камикадзе
class Drone: public Enemy
{
public:
    Drone();

    void update_ai(u64 ns) override;
};
