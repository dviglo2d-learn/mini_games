#include "enemy.hpp"

#include <SDL3/SDL.h>


Enemy::Enemy()
{
    debug_color = 0x900000FF;
}

void Enemy::update_ai(u64 ns)
{
    if (destroyed)
        return;

    lifetime += ns;
}
