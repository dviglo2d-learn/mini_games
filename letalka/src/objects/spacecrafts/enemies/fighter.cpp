#include "fighter.hpp"

#include "../../../world.hpp"
#include "../../projectiles/laser.hpp"
#include "../player.hpp"


Fighter::Fighter()
{
    size = {32.f, 50.f};

    // Просто движется вниз
    velocity = {0.f, 200.f};

    // Начнёт стрелять через некоторое время после вылета на экран
    shoot_delay = SDL_NS_PER_SECOND / 3;
}

void Fighter::update_ai(u64 ns)
{
    if (destroyed)
        return;

    // Начнёт стрелять через некоторое время после вылета на экран
    if (is_inside_screen())
    {
        // Безопасное вычитание, чтобы не было переполнения
        if (shoot_delay >= ns)
            shoot_delay -= ns;
        else
            shoot_delay = 0;

        if (shoot_delay == 0)
        {
            // Создаём лазерный луч
            shared_ptr<Laser> laser = make_shared<Laser>(LaserDir::down);
            vec2 center = calc_center_pos();
            laser->pos = {center.x - laser->size.x / 2, center.y + 20.f};
            WORLD->enemy_projectiles.push_back(laser);

            shoot_delay = SDL_NS_PER_SECOND * 3 / 2;
        }
    }
}

void Fighter::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{182.f, 6.f}, {74.f, 64.f}};
    // Размер спрайта в текстуре
    vec2 sprite_size = uv.size;
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера и смещаем вверх
    vec2 sprite_offset = diff * 0.5f - vec2(0.f, 4.f);
    sprite_batch->draw_sprite(WORLD->spritesheet, pos - sprite_offset, &uv);
}
