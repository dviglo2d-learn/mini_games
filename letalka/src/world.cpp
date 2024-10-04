#include "world.hpp"

#include "global.hpp"

#include "objects/projectiles/projectile.hpp"
#include "objects/spacecrafts/enemies/drone.hpp"
#include "objects/spacecrafts/enemies/fighter.hpp"
#include "objects/spacecrafts/enemies/gunship.hpp"
#include "objects/spacecrafts/player.hpp"

#include <dviglo/fs/fs_base.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>

#include <random>


// Генератор случайных чисел от 0 до 2 (включительно)
static random_device rand_device;
static default_random_engine rand_generator(rand_device());
static uniform_int_distribution<> distrib_0_2(0, 2);

World::World()
{
    assert(!instance_);

    StrUtf8 base_path = get_base_path();
    spritesheet = DV_TEXTURE_CACHE->get(base_path + "letalka_data/textures/spritesheet.png");
    player = make_unique<Player>();
    new_game();

    instance_ = this;
}

World::~World()
{
    instance_ = nullptr;
}

void World::new_game()
{
    // Задержка перед первой пачкой врагов - 1 секунда
    spawn_enemy_delay = SDL_NS_PER_SECOND * 1;

    // Очищаем мир

    for (shared_ptr<Enemy> obj : enemies)
        obj->destroyed = true;

    for (shared_ptr<Projectile> obj : enemy_projectiles)
        obj->destroyed = true;

    for (shared_ptr<Projectile> obj : player_projectiles)
        obj->destroyed = true;

    PLAYER->init();
}

void World::spawn_drones()
{
    // Дрон слева
    shared_ptr<Drone> enemy = make_shared<Drone>();
    enemy->pos = {-enemy->size.x , 100.f};
    enemies.push_back(enemy);

    // Дрон справа
    enemy = make_shared<Drone>();
    enemy->pos = {(f32)fbo_size.x , 100.f};
    enemies.push_back(enemy);
}

void World::spawn_fighters()
{
    // Истребитель за верхней границей экрана
    shared_ptr<Fighter> enemy = make_shared<Fighter>();
    f32 half_width = enemy->size.x * 0.5f;
    uniform_int_distribution<> distrib((i32)half_width, fbo_size.x - (i32)half_width);
    f32 enemy_center_x = (f32)distrib(rand_generator);
    enemy->pos = {enemy_center_x - half_width, -enemy->size.y};
    enemies.push_back(enemy);

    // Второй истребитель выше первого и появится позже
    enemy = make_shared<Fighter>();
    enemy_center_x = (f32)distrib(rand_generator);
    enemy->pos = {enemy_center_x - half_width, -enemy->size.y * 3};
    enemies.push_back(enemy);
}

void World::spawn_gunships()
{
    // Ганшип слева
    shared_ptr<Gunship> enemy = make_shared<Gunship>(true);
    uniform_int_distribution<> distrib(100, 600);
    f32 enemy_y = (f32)distrib(rand_generator);
    enemy->pos = {-enemy->size.x, enemy_y};
    enemies.push_back(enemy);

    // Ганшип справа
    enemy = make_shared<Gunship>(false);
    enemy_y = (f32)distrib(rand_generator);
    enemy->pos = {(f32)fbo_size.x, enemy_y};
    enemies.push_back(enemy);
}

/// Проверяет, что объект пересекается с другим объектом. Уничтоженные объекты не пересекаются
static bool is_collide(const Object* a, const Object* b)
{
    if (a->destroyed || b->destroyed)
        return false;

    if (a->pos.x > b->pos.x + b->size.x
        || a->pos.y > b->pos.y + b->size.y
        || b->pos.x > a->pos.x + a->size.x
        || b->pos.y > a->pos.y + a->size.y)
    {
        return false;
    }

    return true;
}

void World::update(u64 ns)
{
    if (spawn_enemy_delay >= ns) // Безопасное вычитание, чтобы не было переполнения
        spawn_enemy_delay -= ns;
    else
        spawn_enemy_delay = 0;

    if (spawn_enemy_delay == 0)
    {
        i32 enemy_type = distrib_0_2(rand_generator);

        if (enemy_type == 0)
            spawn_drones();
        else if (enemy_type == 1)
            spawn_fighters();
        else // enemy_type == 2
            spawn_gunships();

        spawn_enemy_delay = SDL_NS_PER_SECOND * 2;
    }

    // Апдейтим корабли перед снарядами, так как корабли могут выпустить снаряды, которые
    // нужно проапдейтить в этом же кадре

    player->update_guns(ns); // Положение корабля игрока уже обновлено в App::on_mouse_motion()

    for (shared_ptr<Enemy> obj : enemies)
        obj->update_ai(ns);

    for (shared_ptr<Enemy> obj : enemies)
        obj->update_pos(ns);

    for (shared_ptr<Projectile> obj : enemy_projectiles)
        obj->update_pos(ns);

    for (shared_ptr<Projectile> obj : player_projectiles)
        obj->update_pos(ns);

    // Ищем столкновения снарядов игрока и кораблей противников
    for (shared_ptr<Projectile> proj : player_projectiles)
    {
        for (shared_ptr<Enemy> enemy : enemies)
        {
            if (is_collide(proj.get(), enemy.get()))
            {
                enemy->destroyed = true;
                proj->destroyed = true;
                ++PLAYER->score;
            }
        }
    }

    if (!PLAYER->invulnerable)
    {
        // Ищем столкновения снарядов противника и корабля игрока
        for (shared_ptr<Projectile> proj : enemy_projectiles)
        {
            if (is_collide(proj.get(), PLAYER))
                new_game();
        }

        // Ищем столкновения корбаля игрока с кораблями противника
        for (shared_ptr<Enemy> enemy : enemies)
        {
            if (is_collide(enemy.get(), PLAYER))
                new_game();
        }
    }

    // Чистим списки от уничтоженных объектов
    erase_if(enemies, [](shared_ptr<Enemy> obj) { return obj->destroyed; });
    erase_if(enemy_projectiles, [](shared_ptr<Projectile> obj) { return obj->destroyed; });
    erase_if(player_projectiles, [](shared_ptr<Projectile> obj) { return obj->destroyed; });
}

void World::draw(SpriteBatch* sprite_batch)
{
    for (shared_ptr<Projectile> obj : player_projectiles)
        obj->draw(sprite_batch);

    for (shared_ptr<Projectile> obj : enemy_projectiles)
        obj->draw(sprite_batch);

    for (shared_ptr<Enemy> obj : enemies)
        obj->draw(sprite_batch);

    player->draw(sprite_batch);

    if (debug_draw)
    {
        for (shared_ptr<Projectile> obj : player_projectiles)
            obj->draw_debug(sprite_batch);

        for (shared_ptr<Projectile> obj : enemy_projectiles)
            obj->draw_debug(sprite_batch);

        for (shared_ptr<Enemy> obj : enemies)
            obj->draw_debug(sprite_batch);

        player->draw_debug(sprite_batch);
    }
}
