#pragma once

#include "enemy.hpp"


/// Медленный корабль, стреляющий плазмой
class Gunship : public Enemy
{
public:
    Gunship(bool left);

    void update_ai(u64 ns) override;
    void draw(SpriteBatch* sprite_batch) override;
};
