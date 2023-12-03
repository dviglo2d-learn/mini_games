#pragma once

#include "enemy.hpp"


/// Истребитель, стреляющий лазерами
class Fighter : public Enemy
{
public:
    Fighter();

    void update_ai(u64 ns) override;
    void draw(SpriteBatch* sprite_batch) override;
};
