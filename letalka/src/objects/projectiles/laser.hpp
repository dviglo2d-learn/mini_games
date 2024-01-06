#pragma once

#include "projectile.hpp"


/// В каком направлении летит лазерный луч
enum class LaserDir
{
    up,  ///< Лазер игрока летит вверх
    down ///< Вражеский лазер летит вниз
};

/// Лазерный луч (прямоугольник)
class Laser : public Projectile
{
public:
    Laser(LaserDir laser_dir);

    void draw(SpriteBatch* sprite_batch) override;
};
