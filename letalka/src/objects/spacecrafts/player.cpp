#include "player.hpp"

#include "../projectiles/laser.hpp"
#include "../../world.hpp"

#include <dviglo/main/os_window.hpp>


Player::Player()
{
    assert(!instance_);

    size = {60.f, 40.f};
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
    float half_width = size.x / 2;
    ivec2 screen_size = DV_OS_WINDOW->size_in_pixels();
    float half_screen_width = screen_size.x * 0.5f;
    pos = {half_screen_width - half_width, (float)screen_size.y - size.y - 10.f};
}

void Player::on_mouse_motion(const SDL_MouseMotionEvent& event_data)
{
    vec2 delta{event_data.xrel, event_data.yrel};
    pos += delta;

    // Не позволяем кораблю персонажа выходить за границы экрана
    ivec2 screen_size = DV_OS_WINDOW->size_in_pixels();

    if (pos.x < 0.f)
        pos.x = 0.f;

    if (pos.y < 0.f)
        pos.y = 0.f;

    if (pos.x > (float)screen_size.x - size.x)
        pos.x = (float)screen_size.x - size.x;

    if (pos.y > (float)screen_size.y - size.y)
        pos.y = (float)screen_size.y - size.y;
}

void Player::update_guns(u64 ns)
{
    if (shoot_delay >= ns) // Безопасное вычитание, чтобы не было переполнения
        shoot_delay -= ns;
    else
        shoot_delay = 0;

    float mouse_x, mouse_y;
    Uint32 state = SDL_GetMouseState(&mouse_x, &mouse_x); // TODO: Удалить

    if (shoot_delay == 0 && state & SDL_BUTTON(1))
    {
        // Создаём два лазерных луча
        shared_ptr<Laser> left_laser = make_shared<Laser>(LaserDir::up);
        left_laser->pos = pos;

        shared_ptr<Laser> right_laser = make_shared<Laser>(LaserDir::up);
        right_laser->pos.x = pos.x + size.x - right_laser->size.x;
        right_laser->pos.y = pos.y;

        WORLD->player_projectiles.push_back(left_laser);
        WORLD->player_projectiles.push_back(right_laser);

        shoot_delay = SDL_NS_PER_SECOND / 2;
    }
}
