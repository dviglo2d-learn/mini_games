#include "plasma.hpp"


Plasma::Plasma(vec2 direction)
    : Projectile()
{
    size = {10.f, 10.f};
    direction = normalize(direction); // Гарантируем, что направление нормализовано
    velocity = direction * 100.f;
}
