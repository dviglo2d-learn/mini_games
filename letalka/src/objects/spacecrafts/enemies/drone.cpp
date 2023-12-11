#include "drone.hpp"

#include "../player.hpp"

#include <SDL3/SDL.h>


Drone::Drone()
{
    size = {50.f, 50.f};
}

void Drone::update_ai(u64 ns)
{
    if (destroyed)
        return;

    lifetime += ns;

    constexpr float drone_speed = 100.f;

    // Две секунды наводится на игрока по горизонтали
    if (lifetime < SDL_NS_PER_SECOND * 2)
    {
        float drone_center_x = calc_center_pos().x;
        float player_center_x = PLAYER->calc_center_pos().x;

        if (drone_center_x < player_center_x)
            velocity = {drone_speed, 0.f};
        else
            velocity = {-drone_speed, 0.f};
    }
    else // А потом движется вниз
    {
        velocity = {0.f, drone_speed};
    }
}
