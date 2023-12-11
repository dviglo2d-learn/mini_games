#pragma once

#include "spacecraft.hpp"

#include <SDL3/SDL.h>


/// Космический корабль игрока
class Player : public Spacecraft
{
private:
    /// Инициализируется в конструкторе
    inline static Player* instance_ = nullptr;

public:
    static Player* instance() { return instance_; }

    i32 score;

    Player();
    ~Player() override;

    /// Вызывается в конструкторе и после смерти
    void init();

    /// Реагирует на перемещение мыши
    void on_mouse_motion(const SDL_MouseMotionEvent& event_data);

    /// Стреляет, если нужно
    void update_guns(u64 ns);
};

#define PLAYER (Player::instance())
