#pragma once

#include "../spacecraft.hpp"


/// Базовый класс для всех вражеских кораблей
class Enemy : public Spacecraft
{
public:
    /// Время жизни корабля (в наносекундах).
    /// Влияет на поведение
    u64 lifetime = 0;

    Enemy();

    /// Меняет скорость и испускает снаряды
    virtual void update_ai(u64 ns);
};
