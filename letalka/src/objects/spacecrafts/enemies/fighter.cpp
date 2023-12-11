#include "fighter.hpp"

#include "../../../world.hpp"
#include "../../projectiles/laser.hpp"
#include "../player.hpp"


Fighter::Fighter()
{
    size = {40.f, 60.f};

    // Просто движется вниз
    velocity = {0.f, 200.f};
}

void Fighter::update_ai(u64 ns)
{
    if (destroyed)
        return;

    if (shoot_delay >= ns) // Безопасное вычитание, чтобы не было переполнения
        shoot_delay -= ns;
    else
        shoot_delay = 0;

    if (shoot_delay == 0 && is_inside_screen())
    {
        vec2 center = calc_center_pos();
        vec2 player_center = PLAYER->calc_center_pos();
        vec2 dir = normalize(player_center - center);

        // Создаём лазерный луч
        shared_ptr<Laser> laser = make_shared<Laser>(LaserDir::down);
        laser->pos = center;
        WORLD->enemy_projectiles.push_back(laser);

        shoot_delay = SDL_NS_PER_SECOND;
    }
}
