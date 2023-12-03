#include "laser.hpp"

#include "../../world.hpp"

#include <SDL3/SDL.h>


Laser::Laser(LaserDir laser_dir)
    : Projectile()
{
    size = {4.f, 20.f};

    if (laser_dir == LaserDir::down) // Вражеский лазер летит вниз
        velocity = {0.f, 300.f};
    else // А лазер игрока вверх
        velocity = {0.f, -300.f};
}

void Laser::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{26.f, 114.f}, {128.f, 128.f}};
    // Размер спрайта на экране
    vec2 sprite_size = size;
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера
    vec2 sprite_offset = diff * 0.5f;
    // Позиция спрайта на экране
    vec2 sprite_pos = pos - sprite_offset;
    sprite_batch->draw_sprite(WORLD->spritesheet, Rect(sprite_pos, sprite_size), &uv, 0xAA00FF00);
}
