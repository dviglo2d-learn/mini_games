#include "app.hpp"

#include <dviglo/gl_utils/shader_cache.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>
#include <dviglo/io/fs_base.hpp>
#include <dviglo/main/engine_params.hpp>
#include <dviglo/main/os_window.hpp>

#include <glad/gl.h>

#include <format>


App::App(const vector<StrUtf8>& args)
    : Application(args)
{
    log_path_ = get_pref_path("dviglo2d", "mini_games") + "15_puzzle.log";
}

void App::setup()
{
    engine_params::window_size = ivec2(720, 700);
}

void App::start()
{
    StrUtf8 base_path = get_base_path();
    sprite_batch_ = make_unique<SpriteBatch>();
    spritesheet_ = DV_TEXTURE_CACHE->get(base_path + "15_puzzle_data/textures/spritesheet.png");
    r_20_font_ = make_unique<SpriteFont>(base_path + "samples_data/fonts/ubuntu-r_20_simple.fnt");
    my_font_ = make_unique<SpriteFont>(base_path + "15_puzzle_data/fonts/my_font.fnt");
    puzzle_logic_ = make_shared<PuzzleLogic>();
    puzzle_interface_ = make_shared<PuzzleInterface>(puzzle_logic_);
}

void App::handle_sdl_event(const SDL_Event& event)
{
    Application::handle_sdl_event(event); // Реагируем на закрытие приложения

    switch (event.type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        on_key(event.key);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        on_mouse_button(event.button);
        break;
    }
}

void App::on_key(const SDL_KeyboardEvent& event_data)
{
    if (event_data.type == SDL_EVENT_KEY_DOWN && event_data.repeat == false
        && event_data.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
        should_exit_ = true;
    }
}

static constexpr vec2 new_game_size{238, 59}; // Размер кнопки "Новая игра"
static const Rect new_game_uv{{532, 17}, vec2(532, 17) + new_game_size}; // Участок текстуры с кнопкой
static constexpr vec2 new_game_pos{472, 10}; // Позиция кнопки на экране

void App::on_mouse_button(const SDL_MouseButtonEvent& event_data)
{
    if (event_data.type == SDL_EVENT_MOUSE_BUTTON_DOWN
        && event_data.button == SDL_BUTTON_LEFT)
    {
        vec2 pos{event_data.x, event_data.y};

        if (pos.x >= new_game_pos.x && pos.x < new_game_pos.x + new_game_size.x
            && pos.y >= new_game_pos.y && pos.y < new_game_pos.y + new_game_size.y)
        {
            puzzle_logic_->new_game();
            return;
        }

        puzzle_interface_->on_click({event_data.x, event_data.y});
    }
}

static StrUtf8 fps_text;

void App::update(u64 ns)
{
    u64 fps = SDL_NS_PER_SECOND / ns;
    fps_text = format("FPS: {}", fps);
}

void App::draw()
{
    ivec2 screen_size = DV_OS_WINDOW->size_in_pixels();

    // Закрашиваем фон
    glClearColor(0.f, 0.3f, 0.3f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Задаём треугольники по часовой стрелке
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Включаем альфа-смешение
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Рисуем игровое поле
    puzzle_interface_->draw(sprite_batch_.get());

    // Рисуем кнопку "Новая игра"
    sprite_batch_->draw_sprite(spritesheet_, new_game_pos, &new_game_uv);

    if (puzzle_logic_->check_win())
        sprite_batch_->draw_string("Победа!", my_font_.get(), vec2{200.f, 340.f});

    // Рисуем счётчик ФПС с тенью
    sprite_batch_->draw_string(fps_text, r_20_font_.get(), vec2{4.f, 1.f}, 0xFF000000);
    sprite_batch_->draw_string(fps_text, r_20_font_.get(), vec2{3.f, 0.f}, 0xFFFFFFFF);

    // Выводим остаток накопленных спрайтов
    sprite_batch_->flush();
}
