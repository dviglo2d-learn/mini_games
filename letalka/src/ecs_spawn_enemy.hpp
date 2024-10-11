#pragma once

#include <dviglo/common/primitive_types.hpp>
#include <entt/entt.hpp>

using namespace dviglo;
using namespace entt;


// ===================== Компоненты =====================

// Задержка перед созданием очередной пачки врагов
struct CSpawnEnemyDelay
{
    u64 value = 0;
};

// ===================== Системы =====================

// Создаёт очередную пачку врагов, если прошла задержка
void s_spawn_enemy(u64 ns);

// ===================== Утилиты =====================

void create_enemy_spawner();
void reset_enemy_spawner();
