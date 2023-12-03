#include "plasma.hpp"

#include "../../world.hpp"


Plasma::Plasma(vec2 direction)
    : Projectile()
{
    size = {10.f, 10.f};
    direction = normalize(direction); // Гарантируем, что направление нормализовано
    velocity = direction * 100.f;
}

void Plasma::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{26.f, 114.f}, {128.f, 128.f}};
    // Размер спрайта на экране
    vec2 sprite_size{12.f, 12.f};
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера
    vec2 sprite_offset = diff * 0.5f;
    // Позиция спрайта на экране
    vec2 sprite_pos = pos - sprite_offset;
    sprite_batch->draw_sprite(WORLD->spritesheet, Rect(sprite_pos, sprite_size), &uv, 0xAA00BBFF);
}
