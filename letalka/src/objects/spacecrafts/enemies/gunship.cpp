#include "gunship.hpp"

#include "../../../world.hpp"
#include "../../projectiles/plasma.hpp"
#include "../player.hpp"

#include <memory>

using namespace std;


Gunship::Gunship(bool left)
{
    size = {50.f, 50.f};

    if (left) // Появляется слева - движемся вправо
        velocity = {300.f, 0.f};
    else
        velocity = {-300.f, 0.f};

    // Начнёт стрелять через некоторое время после вылета на экран
    shoot_delay = SDL_NS_PER_SECOND / 2;
}

void Gunship::update_ai(u64 ns)
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
            vec2 center = calc_center_pos();
            vec2 player_center = PLAYER->calc_center_pos();
            vec2 dir = normalize(player_center - center);

            // Создаём плазму
            shared_ptr<Plasma> plasma = make_shared<Plasma>(dir);
            plasma->pos = center;
            WORLD->enemy_projectiles.push_back(plasma);

            shoot_delay = SDL_NS_PER_SECOND * 2;
        }
    }
}

void Gunship::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{269.f, 7.f}, {64.f, 64.f}};
    // Размер спрайта в текстуре
    vec2 sprite_size = uv.size;
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера
    vec2 sprite_offset = diff * 0.5f;
    sprite_batch->draw_sprite(WORLD->spritesheet, pos - sprite_offset, &uv);
}
