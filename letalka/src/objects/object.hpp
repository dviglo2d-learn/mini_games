#pragma once

#include <dviglo/common/primitive_types.hpp>
#include <dviglo/graphics/sprite_batch.hpp>
#include <dviglo/math/rect.hpp>

#include <glm/glm.hpp>

using namespace dviglo;
using namespace glm;


/// Физический объект, у которого есть размер, координаты и скорость
class Object
{
public:
    /// Положение верхнего левого угла коллайлдера объекта
    /// (в игре нет разделения на мировые и экранные координаты)
    vec2 pos{0.f, 0.f};

    /// Размер коллайдера объекта (в пикселях)
    vec2 size{10.f, 10.f};

    /// Скорость (пикселей в секунду)
    vec2 velocity{0.f, 0.f};

    /// Цвет прямоугольника при рисовании коллайдера
    u32 debug_color = 0x90FFFFFF;

    /// Уничтоженный объект не апдейтится и не рендерится. Он будет удалён из списков, когда будет возможно
    bool destroyed = false;

    virtual ~Object() = default;

    vec2 calc_center_pos();

    /// Проверяет, что никакая часть объекта не находится за пределами экрана
    bool is_inside_screen();

    /// Перемещает объект и помечает уничтоженным, если он далеко за пределами экрана
    virtual void update_pos(u64 ns);

    virtual void draw(SpriteBatch* sprite_batch) {}

    void draw_debug(SpriteBatch* sprite_batch);
};
