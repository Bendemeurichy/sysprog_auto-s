#ifndef OPENGL_GAME_RENDERER_H
#define OPENGL_GAME_RENDERER_H

#include "sdl_graphics.h"
#include "spgsdl_texture.h"

#include <glmc.h>

typedef uint16_t TileId;
#define TILE_ID_NONE 0xFFFF
#define TILE_MAX 1500

typedef struct Tile {
    TextureId texture_id;

    uint16_t x, y, w, h; //offset and dimensions in sprite sheet
    int8_t offset_x, offset_y; //before rendering, always apply this offset  (only used on tiles that are "shifted")
} Tile;

typedef struct RendererIsometric {
    Graphics graphics;

    t_vec3 screen_center_pos;  //3d iso grid coordinate in the center of the screen
    t_ivec2 screen_translate;  //isometric screen translate, based on screen_center_pos

    //The "diamond" is the base plate of the tile.
    //The tile itself typically sticks out above it,
    // but the dimensions of the base diamond are used for lots of calculations.
    uint8_t diamond_width;
    uint8_t diamond_height;
    uint8_t tile_layer_height; //height of 1 "level" in the tile texture

    Tile tiles[TILE_MAX];
    size_t tile_count;

    TileId tile_texture_offset[TEXTURE__COUNT];
    size_t tile_texture_count[TEXTURE__COUNT];
} RendererIsometric;

void renderer_isometric_init(RendererIsometric*, int width, int height);
void renderer_isometric_clear(RendererIsometric*);
void renderer_isometric_free(RendererIsometric*);

void renderer_isometric_begin_draw(RendererIsometric*);
void renderer_isometric_end_draw(RendererIsometric*);

void pos_isometric_to_screen(RendererIsometric*, const t_vec3 src_iso_pos, t_ivec2 dest_screen_pos);
void pos_screen_to_isometric(RendererIsometric*, const t_ivec2 source, t_vec2 dest_iso_pos); //this cannot determine z!
void pos_isometric_flatten(RendererIsometric* renderer, const t_vec3 src_iso_pos, t_vec2 dest_iso_pos);

void pos_isometric_to_3d(RendererIsometric*, t_vec3 source, t_vec3 dest);
void pos_3d_to_isometric(RendererIsometric*, t_vec3 source, t_vec3 dest);

void renderer_isometric_draw_tile(RendererIsometric* renderer, TileId tile_id, t_vec3 iso_pos);

void renderer_isometric_grid(RendererIsometric* renderer, int center, float z, uint8_t r, uint8_t g, uint8_t b);
void renderer_isometric_tile_border(RendererIsometric* renderer, t_vec3 iso_pos, uint8_t r, uint8_t g, uint8_t b, uint8_t a) ;
void renderer_isometric_rect(RendererIsometric* renderer, t_vec3 iso_pos0, t_vec3 iso_pos1, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#endif //OPENGL_GAME_RENDERER_H
