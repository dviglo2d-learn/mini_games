#include "drone.hpp"

#include "../player.hpp"
#include "../../../world.hpp"

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

    constexpr f32 drone_speed = 100.f;

    // Две секунды наводится на игрока по горизонтали
    if (lifetime < SDL_NS_PER_SECOND * 2)
    {
        f32 drone_center_x = calc_center_pos().x;
        f32 player_center_x = PLAYER->calc_center_pos().x;

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

void Drone::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{108.f, 2.f}, {64.f, 64.f}};
    // Размер спрайта в текстуре
    vec2 sprite_size = uv.size;
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера
    vec2 sprite_offset = diff * 0.5f;
    sprite_batch->draw_sprite(WORLD->spritesheet, pos - sprite_offset, &uv);
}
