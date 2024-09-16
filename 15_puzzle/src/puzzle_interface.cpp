#include "puzzle_interface.hpp"

#include <dviglo/fs/fs_base.hpp>
#include <dviglo/gl_utils/texture_cache.hpp>


/// Ширина и высота текстуры коробки в пикселях
static constexpr f32 box_size = 512.f;

/// Ширина и высота текстуры костяшки в пикселях
static constexpr f32 tile_size = 118.f;

/// Верхний левый угол поля
static constexpr vec2 puzzle_pos{100.f, 124.f};

/// Ширина рамки поля
static constexpr f32 border_size = 19.0f;

/// Зазор между костяшками при рендеринге
static constexpr f32 tile_gap = 1.f;

/// Возвращает участок текстуры, на котором находится костяшка с указанным числом.
/// value может быть 0 ... 15
static Rect calc_tile_uv(u8 value)
{
    assert(value <= 15);

    i32 index_x = value % 4; // Столбец
    i32 index_y = value / 4; // Строка

    // В текстуре промежуток между костяшками - 2 пикселя
    f32 u = 3.f + index_x * tile_size + 2.f * index_x; // Левая граница костяшки
    f32 v = 514.f + index_y * tile_size + 2.f * index_y; // Верхняя граница костяшки

    return {{u, v}, {tile_size, tile_size}}; // Правая и нижняя граница включаются в Rect
}

PuzzleInterface::PuzzleInterface(weak_ptr<PuzzleLogic> logic)
    : logic_(logic)
{
    StrUtf8 base_path = get_base_path();
    spritesheet_ = DV_TEXTURE_CACHE->get(base_path + "15_puzzle_data/textures/spritesheet.png");

    StrUtf8 sound_path = base_path + "15_puzzle_data/sounds/tile_move.wav";
    tile_move_sound_ = Mix_LoadWAV(sound_path.c_str());
}

PuzzleInterface::~PuzzleInterface()
{
    Mix_FreeChunk(tile_move_sound_); // Проверка на nullptr не нужна
    tile_move_sound_ = nullptr;
}

void PuzzleInterface::on_click(vec2 mouse_pos)
{
    // Координаты мыши относительно верхнего левого угла первой клетки
    vec2 local_pos = mouse_pos - puzzle_pos - vec2(border_size, border_size);
    // Пусть щелчок по вертикальному зазору относится к левой клетке
    i32 index_x = (i32)floor(local_pos.x / (tile_size + tile_gap));
    // Пусть щелчок по горизонтальному зазору относится к верхней клетке
    i32 index_y = (i32)floor(local_pos.y / (tile_size + tile_gap));

    if (index_x >= 0 || index_x <= 3 || index_y >= 0 || index_y <= 3)
    {
        shared_ptr<PuzzleLogic> logic = logic_.lock();
        if (logic->move({index_x, index_y}))
            Mix_PlayChannel(-1, tile_move_sound_, 0);
    }
}

void PuzzleInterface::draw(SpriteBatch* sprite_batch)
{
    shared_ptr<PuzzleLogic> logic = logic_.lock();

    // Рисуем коробку
    Rect box_uv{{0.f, 0.f}, {box_size, box_size}};
    sprite_batch->draw_sprite(spritesheet_, puzzle_pos, &box_uv);

    // Рисуем костяшки
    for (i32 index_y = 0; index_y < 4; ++index_y)
    {
        for (i32 index_x = 0; index_x < 4; ++index_x)
        {
            f32 tile_pos_x = puzzle_pos.x + border_size + tile_size * index_x + tile_gap * index_x;
            f32 tile_pos_y = puzzle_pos.y + border_size + tile_size * index_y + tile_gap * index_y;
            Rect tile_uv = calc_tile_uv(logic->get_tile({index_x, index_y}));
            sprite_batch->draw_sprite(spritesheet_, {tile_pos_x, tile_pos_y}, &tile_uv);
        }
    }
}
