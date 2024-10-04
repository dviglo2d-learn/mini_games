#include "player.hpp"

#include "../projectiles/laser.hpp"
#include "../../global.hpp"
#include "../../world.hpp"


Player::Player()
{
    assert(!instance_);

    size = {80.f, 60.f};
    debug_color = 0x9000FF00;
    init();

    instance_ = this;
}

Player::~Player()
{
    instance_ = nullptr;
}

void Player::init()
{
    shoot_delay = 0;
    score = 0;

    // Размещаем игрока в центре низа экрана
    f32 half_width = size.x / 2;
    f32 half_fbo_width = fbo_size.x * 0.5f;
    pos = {half_fbo_width - half_width, (f32)fbo_size.y - size.y - 10.f};
}

void Player::on_mouse_motion(const SDL_MouseMotionEvent& event_data)
{
    vec2 delta{event_data.xrel, event_data.yrel};
    pos += delta;

    // Не позволяем кораблю персонажа выходить за границы экрана

    if (pos.x < 0.f)
        pos.x = 0.f;

    if (pos.y < 0.f)
        pos.y = 0.f;

    if (pos.x > (f32)fbo_size.x - size.x)
        pos.x = (f32)fbo_size.x - size.x;

    if (pos.y > (f32)fbo_size.y - size.y)
        pos.y = (f32)fbo_size.y - size.y;
}

void Player::update_guns(u64 ns)
{
    if (shoot_delay >= ns) // Безопасное вычитание, чтобы не было переполнения
        shoot_delay -= ns;
    else
        shoot_delay = 0;

    f32 mouse_x, mouse_y;
    Uint32 state = SDL_GetMouseState(&mouse_x, &mouse_x); // TODO: Удалить

    if (shoot_delay == 0 && state & SDL_BUTTON(1))
    {
        // Создаём два лазерных луча
        shared_ptr<Laser> left_laser = make_shared<Laser>(LaserDir::up);
        left_laser->pos = pos + vec2(6.f, -12.f);

        shared_ptr<Laser> right_laser = make_shared<Laser>(LaserDir::up);
        right_laser->pos.x = pos.x + size.x - right_laser->size.x - 6.f;
        right_laser->pos.y = pos.y - 12.f;

        WORLD->player_projectiles.push_back(left_laser);
        WORLD->player_projectiles.push_back(right_laser);

        shoot_delay = SDL_NS_PER_SECOND / 2;
    }
}

void Player::draw(SpriteBatch* sprite_batch)
{
    // Область спрайта в текстуре
    Rect uv{{0.f, 0.f}, {100.f, 100.f}};
    // Размер спрайта в текстуре
    vec2 sprite_size = uv.size;
    // Разница между размером спрайта и размером коллайдера
    vec2 diff = sprite_size - size;
    // Центрируем спрайт относительно коллайдера и смещаем вверх
    vec2 sprite_offset = diff * 0.5f + vec2(0.f, 10.f);
    sprite_batch->draw_sprite(WORLD->spritesheet, pos - sprite_offset, &uv);
}
