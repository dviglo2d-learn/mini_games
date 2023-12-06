#pragma once

#include <dviglo/graphics/sprite_batch.hpp>
#include <dviglo/main/application.hpp>

using namespace dviglo;
using namespace std;


class App : public Application
{
    Texture* texture_;
    unique_ptr<SpriteBatch> sprite_batch_;
    unique_ptr<SpriteFont> r_20_font_;

public:
    App(const vector<StrUtf8>& args);

    void setup() override;
    void start() override;
    void update(u64 ns) override;
    void draw() override;
};
