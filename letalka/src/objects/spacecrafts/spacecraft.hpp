#pragma once

#include "../object.hpp"


/// Базовый класс для всех космических аппаратов
class Spacecraft : public Object
{
public:
    /// Остаток времени до следующего выстрела (в наносекундах)
    u64 shoot_delay = 0;
};
