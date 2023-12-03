#pragma once

#include "projectile.hpp"


/// Шарик (на самом деле квадратик)
class Plasma : public Projectile
{
public:
    Plasma(vec2 direction);

    void draw(SpriteBatch* sprite_batch) override;
};
