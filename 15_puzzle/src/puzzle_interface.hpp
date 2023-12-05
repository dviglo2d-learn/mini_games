#pragma once

#include "puzzle_logic.hpp"

#include <dviglo/gl_utils/texture.hpp>
#include <dviglo/graphics/sprite_batch.hpp>


/// Обработка ввода и рендеринг игрового поля
class PuzzleInterface
{
private:
    Texture* spritesheet_;
    weak_ptr<PuzzleLogic> logic_;

public:
    PuzzleInterface(weak_ptr<PuzzleLogic> logic);

    void on_click(vec2 mouse_pos);
    void draw(SpriteBatch* sprite_batch);
};
