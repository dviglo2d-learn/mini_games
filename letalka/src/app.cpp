#include "app.hpp"

#include "ecs_main.hpp"

#include <dviglo/fs/fs_base.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>
#include <dviglo/main/engine_params.hpp>
#include <dviglo/main/os_window.hpp>

#include <format>


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

    global_ = make_unique<Global>();

    ecs_start();

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
        ecs_on_mouse_motion(event.motion);
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
        global_->debug_draw = !global_->debug_draw;
    }

    if (event_data.type == SDL_EVENT_KEY_DOWN && event_data.repeat == false
        && event_data.scancode == SDL_SCANCODE_F3)
    {
        global_->god_mode = !global_->god_mode;
    }
}

void App::update(u64 ns)
{
    ecs_update(ns);
}

void App::draw()
{
    // Рендерим игру в текстуру
    fbo_->bind();
    glViewport(0, 0, fbo_size.x, fbo_size.y);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    global_->sprite_batch()->prepare_ogl(true, true);
    ecs_draw();
    global_->sprite_batch()->flush();
    fbo_->texture()->bind();
    glGenerateMipmap(GL_TEXTURE_2D);

    // Возвращаемся к рендерингу в default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Очищаем всё окно
    glClearColor(0.f, 0.05f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Вычисляем положение вьюпорта в окне

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

    // Выводим отрендеренную текстуру в окно
#if true
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_->gpu_object_name());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // 0 - default framebuffer
    glBlitFramebuffer(0, 0, fbo_size.x, fbo_size.y,
                      viewport_pos.x, viewport_pos.y + viewport_size.y, // Отражаем по вертикали
                      viewport_pos.x + viewport_size.x, viewport_pos.y, // Отражаем по вертикали
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
#else // А можно так
    glViewport(viewport_pos.x, viewport_pos.y, viewport_size.x, viewport_size.y);
    global_->sprite_batch()->prepare_ogl(false, false);
    global_->sprite_batch()->draw_sprite(fbo_->texture(), Rect(0.f, 0.f, viewport_size.x, viewport_size.y));
    global_->sprite_batch()->flush();
#endif
}
