#include "laser.hpp"

#include <SDL3/SDL.h>


Laser::Laser(LaserDir laser_dir)
    : Projectile()
{
    size = {4.f, 20.f};

    if (laser_dir == LaserDir::down) // Вражеский лазер летит вниз
        velocity = {0.f, 300.f};
    else // А лазер игрока вверх
        velocity = {0.f, -300.f};
}
