#include "app.hpp"

#include "objects/spacecrafts/player.hpp"

#include <dviglo/fs/fs_base.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>
#include <dviglo/main/engine_params.hpp>
#include <dviglo/main/os_window.hpp>

#include <format>

using namespace glm;


App::App(const vector<StrUtf8>& args)
    : Application(args)
{
}

void App::setup()
{
    engine_params::log_path = get_pref_path("dviglo2d", "mini_games") + "letalka.log";
    engine_params::window_size = ivec2(900, 700);
    engine_params::window_title = "Леталка";
    engine_params::vsync = -1;
}

void App::start()
{
    SDL_SetWindowRelativeMouseMode(DV_OS_WINDOW->window(), SDL_TRUE);

    StrUtf8 base_path = get_base_path();
    sprite_batch_ = make_unique<SpriteBatch>();
    r_20_font_ = make_unique<SpriteFont>(base_path + "samples_data/fonts/ubuntu-r_20_simple.fnt");
    world_ = make_unique<World>();
}

bool App::handle_sdl_event(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        on_key(event.key);
        return true;

    case SDL_EVENT_MOUSE_MOTION:
        on_mouse_motion(event.motion);
        return true;

    default:
        // Реагируем на закрытие приложения и изменение размера окна
        return Application::handle_sdl_event(event);
    }
}

void App::on_key(const SDL_KeyboardEvent& event_data)
{
    if (event_data.type == SDL_EVENT_KEY_DOWN && event_data.repeat == false
        && event_data.scancode == SDL_SCANCODE_ESCAPE)
    {
        should_exit_ = true;
    }

    if (event_data.type == SDL_EVENT_KEY_DOWN && event_data.repeat == false
        && event_data.scancode == SDL_SCANCODE_F2)
    {
        WORLD->debug_draw = !WORLD->debug_draw;
    }

    if (event_data.type == SDL_EVENT_KEY_DOWN && event_data.repeat == false
        && event_data.scancode == SDL_SCANCODE_F3)
    {
        PLAYER->invulnerable = !PLAYER->invulnerable;
    }
}

void App::on_mouse_motion(const SDL_MouseMotionEvent& event_data)
{
    PLAYER->on_mouse_motion(event_data);
}

void App::update(u64 ns)
{
    WORLD->update(ns);
}

void App::draw()
{
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    sprite_batch_->prepare_ogl();

    WORLD->draw(sprite_batch_.get());

    StrUtf8 score_text = "Счёт: " + to_string(PLAYER->score);

    sprite_batch_->draw_string(score_text, r_20_font_.get(), vec2{4.f, 1.f}, 0xFF000000);
    sprite_batch_->draw_string(score_text, r_20_font_.get(), vec2{3.f, 0.f}, 0xFFFFFFFF);

    sprite_batch_->flush();
}
