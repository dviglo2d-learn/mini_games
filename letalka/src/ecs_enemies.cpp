#include "ecs_enemies.hpp"

#include "ecs_player.hpp"
#include "global.hpp"


void s_update_drone_velocities(u64 ns)
{
    registry& reg = *GLOBAL->reg();

    auto view = reg.view<CObject, CEnemy, CVelocity>(exclude<CDestroyedMarker>);

    for (entity ent : view)
    {
        CObject& obj = view.get<CObject>(ent);
        CEnemy& enemy = view.get<CEnemy>(ent);
        CVelocity& velocity = view.get<CVelocity>(ent);

        enemy.lifetime += ns;

        // Поведение дрона
        if (reg.all_of<CDroneMarker>(ent))
        {
            constexpr f32 drone_speed = 100.f;

            // Три секунды наводится на игрока по горизонтали
            if (enemy.lifetime < ns_per_second * 3)
            {
                entity player_ent = get_player();
                CObject& player_obj = reg.get<CObject>(player_ent);

                f32 drone_center_x = obj.pos.x;
                f32 player_center_x = player_obj.pos.x;

                if (drone_center_x < player_center_x)
                    velocity.value = vec2(drone_speed, 0.f);
                else
                    velocity.value = vec2(-drone_speed, 0.f);
            }
            else // А потом движется вниз
            {
                velocity.value = vec2(0.f, drone_speed);
            }
        }
    }
}

void s_draw_enemies()
{
    registry& reg = *GLOBAL->reg();
    SpriteBatch& sprite_batch = *GLOBAL->sprite_batch();

    auto view = reg.view<CObject, CEnemy>();
    for (entity ent : view)
    {
        CObject& obj = view.get<CObject>(ent);
        sprite_batch.draw_sprite(GLOBAL->spritesheet(), obj.pos - obj.uv.size * 0.5f, &obj.uv);
    }
}

void s_draw_enemy_hitboxes()
{
    registry& reg = *GLOBAL->reg();
    SpriteBatch& sprite_batch = *GLOBAL->sprite_batch();

    auto view = reg.view<CObject, CEnemy>();
    for (auto [ent, obj, enemy] : view.each())
    {
        sprite_batch.set_shape_color(0x60FFFFFF);
        sprite_batch.draw_rect({obj.pos + enemy.hitbox.pos - enemy.hitbox.half_size,
                                enemy.hitbox.half_size * 2.f});
    }
}
