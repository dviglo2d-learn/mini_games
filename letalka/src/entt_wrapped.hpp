// Нужно подключать этот файл вместо <entt/entt.hpp>.
// Подавляет предупреждение 'meta_prop' is deprecated: use meta_custom instead

#pragma once

#ifdef ENTT_VERSION
#error "Don't include <entt/entt.hpp>"
#endif

#if defined(__GNUC__) // GCC, Clang или MinGW
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <entt/entt.hpp>

#if defined(__GNUC__) // GCC, Clang или MinGW
#pragma GCC diagnostic pop
#endif
