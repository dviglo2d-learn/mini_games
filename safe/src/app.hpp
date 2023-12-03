#pragma once

#include <dviglo/gl_utils/index_buffer.hpp>
#include <dviglo/gl_utils/shader_program.hpp>
#include <dviglo/gl_utils/texture.hpp>
#include <dviglo/gl_utils/vertex_buffer.hpp>
#include <dviglo/graphics/sprite_batch.hpp>
#include <dviglo/graphics/sprite_font.hpp>
#include <dviglo/main/application.hpp>

using namespace dviglo;
using namespace std;


class App : public Application
{
    ShaderProgram* textured_shader_;
    Texture* texture_;
    ShaderProgram* vert_color_shader_prog_;
    unique_ptr<SpriteBatch> sprite_batch_;
    unique_ptr<SpriteFont> r_20_font_;

public:
    App(const vector<StrUtf8>& args);

    void setup() override;
    void start() override;
    void update(u64 ns) override;
    void draw() override;
};
