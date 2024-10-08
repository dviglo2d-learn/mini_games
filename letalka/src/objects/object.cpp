#include "object.hpp"

#include "../global.hpp"

#include <SDL3/SDL.h>


vec2 Object::calc_center_pos()
{
    return pos + size * 0.5f;
}

bool Object::is_inside_screen()
{
    if (pos.x < 0 || pos.y < 0)
        return false;

    if (pos.x > (f32)fbo_size.x - size.x || pos.y > (f32)fbo_size.y - size.y)
        return false;

    return true;
}

void Object::update_pos(u64 ns)
{
    if (destroyed)
        return;

    // Двигаем объект
    f32 seconds = (f32)ns / SDL_NS_PER_SECOND;
    pos += velocity * seconds;

    // Уничтожаем корабли и снаряды, которые находятся далеко за пределами экрана.
    // Объекты, которые находятся рядом с краем экрана, уничтожать не надо, так как
    // вражеские корабли создаются за границей экрана
    
    // Область, в которой не будут уничтожаться объекты, больше области экрана
    const f32 border_size = 500.f;

    if (pos.x + size.x < -border_size || pos.y + size.y < -border_size
        || pos.x > (f32)fbo_size.x + border_size || pos.y > (f32)fbo_size.y + border_size)
    {
        destroyed = true;
    }
}

void Object::draw_debug(SpriteBatch* sprite_batch)
{
    if (destroyed)
        return;

    sprite_batch->set_shape_color(debug_color);
    sprite_batch->draw_rect({pos, size});
}
