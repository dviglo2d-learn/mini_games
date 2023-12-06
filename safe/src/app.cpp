#include "app.hpp"

#include <dviglo/gl_utils/shader_cache.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>
#include <dviglo/io/fs_base.hpp>
#include <dviglo/main/engine_params.hpp>
#include <dviglo/main/os_window.hpp>

#include <glad/gl.h>
#include <glm/glm_wrapped.hpp>

#include <format>
#include <iostream>

using namespace glm;


App::App(const vector<StrUtf8>& args)
    : Application(args)
{
    cout << "Командная строка: " << join(args, " ") << endl;

    log_path_ = get_pref_path("dviglo2d", "mini_games") + "safe.log";
}

struct Vertex
{
    vec2 pos;
    u32 color;
    vec2 uv;
};

void App::setup()
{
    engine_params::window_size = ivec2(900, 700);
}

void App::start()
{
    StrUtf8 base_path = get_base_path();
    cout << "Папка программы: " << base_path << endl;

    texture_ = DV_TEXTURE_CACHE->get(base_path + "samples_data/textures/tile128.png");
    sprite_batch_ = make_unique<SpriteBatch>();
    r_20_font_ = make_unique<SpriteFont>(base_path + "samples_data/fonts/ubuntu-r_20_simple.fnt");
}

static float rotation = 0.f;
static StrUtf8 fps_text = "FPS: ?";

void App::update(u64 ns)
{
    static u64 frame_counter = 0;
    static u64 time_counter = 0;

    ++frame_counter;
    time_counter += ns;

    // Обновляем fps_text каждые пол секунды
    if (time_counter >= SDL_NS_PER_SECOND / 2)
    {
        u64 fps = frame_counter * SDL_NS_PER_SECOND / time_counter;
        fps_text = format("FPS: {}", fps);
        frame_counter = 0;
        time_counter = 0;
    }

    rotation += ns * 0.000'000'000'1f;
    while (rotation >= 360.f)
        rotation -= 360.f;
}

void App::draw()
{
    ivec2 screen_size = DV_OS_WINDOW->size_in_pixels();

    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sprite_batch_->prepare_ogl();

    sprite_batch_->triangle_.v0 = {{800.f, 0.f}, 0xFF00FF00};
    sprite_batch_->triangle_.v1 = {{800.f, 300.f}, 0xFF0000FF};
    sprite_batch_->triangle_.v2 = {{0.f, 300.f}, 0xFFFFFFFF};
    sprite_batch_->add_triangle();

    sprite_batch_->set_shape_color(0xFFFF0000);
    sprite_batch_->draw_triangle({400.f, 0.f}, {400.f, 600.f}, {0.f, 600.f});

    sprite_batch_->draw_sprite(texture_, {100.f, 100.f});
    sprite_batch_->draw_sprite(texture_, {500.f, 100.f}, nullptr, 0xFFFFFFFF, rotation);

    sprite_batch_->draw_string(fps_text, r_20_font_.get(), vec2{4.f, 1.f}, 0xFF000000);
    sprite_batch_->draw_string(fps_text, r_20_font_.get(), vec2{3.f, 0.f}, 0xFFFFFFFF);

    sprite_batch_->flush();
}
