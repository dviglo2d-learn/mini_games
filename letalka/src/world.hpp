#pragma once

#include <dviglo/graphics/sprite_batch.hpp>

#include <SDL3/SDL.h>

#include <memory>
#include <vector>

using namespace dviglo;
using namespace std;


class Player;
class Enemy;
class Projectile;

class World
{
private:
    /// Инициализируется в конструкторе
    inline static World* instance_ = nullptr;

public:
    static World* instance() { return instance_; }

    /// Рисовать ли коллайдеры объектов
    bool debug_draw = false;

    Texture* spritesheet;

    /// Задержка перед созданием очередной пачки врагов
    u64 spawn_enemy_delay;

    unique_ptr<Player> player; // Создаётся в конструкторе
    vector<shared_ptr<Enemy>> enemies;
    vector<shared_ptr<Projectile>> player_projectiles;
    vector<shared_ptr<Projectile>> enemy_projectiles;

    World();
    ~World();

    void new_game();

    void spawn_drones();
    void spawn_fighters();
    void spawn_gunships();

    void update(u64 ns);
    void draw(SpriteBatch* sprite_batch);
};

#define WORLD (World::instance())
