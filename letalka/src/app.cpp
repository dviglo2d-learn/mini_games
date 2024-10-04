#include "app.hpp"

#include "global.hpp"
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
    engine_params::window_mode = WindowMode::fullscreen_window;
}

void App::start()
{
    SDL_SetWindowRelativeMouseMode(DV_OS_WINDOW->window(), true);

    StrUtf8 base_path = get_base_path();
    sprite_batch_ = make_unique<SpriteBatch>();
    r_20_font_ = make_unique<SpriteFont>(base_path + "samples_data/fonts/ubuntu-r_20_simple.fnt");
    world_ = make_unique<World>();

    fbo_ = make_unique<Fbo>(fbo_size);
    fbo_->texture()->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void App::handle_sdl_event(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        on_key(event.key);
        return;

    case SDL_EVENT_MOUSE_MOTION:
        on_mouse_motion(event.motion);
        return;

    default:
        // Реагируем на закрытие приложения и изменение размера окна
        Application::handle_sdl_event(event);
        return;
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
    // Рендерим игру в текстуру
    fbo_->bind();
    glViewport(0, 0, fbo_size.x, fbo_size.y);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    sprite_batch_->prepare_ogl(true, true);
    WORLD->draw(sprite_batch_.get());

    StrUtf8 score_text = "Счёт: " + to_string(PLAYER->score);
    sprite_batch_->draw_string(score_text, r_20_font_.get(), vec2{4.f, 1.f}, 0xFF000000);
    sprite_batch_->draw_string(score_text, r_20_font_.get(), vec2{3.f, 0.f}, 0xFFFFFFFF);

    sprite_batch_->flush();
    fbo_->texture()->bind();
    glGenerateMipmap(GL_TEXTURE_2D);

    // Возвращаемся к рендерингу в default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Очищаем всё окно
    glClearColor(0.f, 0.05f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Настраиваем вьюпорт в окне

    ivec2 window_size;
    SDL_GetWindowSizeInPixels(DV_OS_WINDOW->window(), &window_size.x, &window_size.y);
    f32 real_aspect = (f32)window_size.x / window_size.y;
    f32 fbo_aspect = (f32)fbo_size.x / fbo_size.y;

    f32 fbo_scale;
    if (real_aspect > fbo_aspect)
    {
        // Окно шире, чем надо. Будут пустые полосы по бокам
        fbo_scale = (f32)window_size.y / fbo_size.y;
    }
    else
    {
        // Высота окна больше, чем надо. Будут пустые полосы сверху и снизу
        fbo_scale = (f32)window_size.x / fbo_size.x;
    }

    ivec2 viewport_size(
        (i32)(fbo_size.x * fbo_scale),
        (i32)(fbo_size.y * fbo_scale)
    );

    // Центрируем вьюпорт
    ivec2 viewport_pos(
        (window_size.x - viewport_size.x) / 2,
        (window_size.y - viewport_size.y) / 2
    );

    glViewport(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);

    // Выводим отрендеренную текстуру в окно
    sprite_batch_->prepare_ogl(false, false);
    sprite_batch_->draw_sprite(fbo_->texture(), Rect(0.f, 0.f, viewport_size.x, viewport_size.y));
    sprite_batch_->flush();
}
