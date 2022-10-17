#include "renderer_isometric.h"


#include <SDL_timer.h>
#include <assert.h>

#include <glmc.h>

TileId add_tile(RendererIsometric *renderer, TextureId texture_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    TileId new_id = renderer->tile_count++;
    assert(new_id >= 0);
    assert(new_id < renderer->tile_count);
    assert(new_id < TILE_MAX);

    renderer->tiles[new_id].texture_id = texture_id;
    renderer->tiles[new_id].x = x;
    renderer->tiles[new_id].y = y;
    renderer->tiles[new_id].w = w == 0 ? renderer->graphics.textures[texture_id].width : w;
    renderer->tiles[new_id].h = h == 0 ? renderer->graphics.textures[texture_id].height : h;
    renderer->tiles[new_id].offset_x = 0;
    renderer->tiles[new_id].offset_y = 0;

    if (renderer->tile_texture_count[texture_id]++ == 0) {
        renderer->tile_texture_offset[texture_id] = new_id;
    }

    return new_id;
}

TileId add_center_tile(RendererIsometric *renderer, TextureId texture_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    TileId new_id = renderer->tile_count++;
    assert(new_id >= 0);
    assert(new_id < renderer->tile_count);
    assert(new_id < TILE_MAX);

    //find offset needed for car tile
    //middle of car tile should be renderer->diamond_width/2  renderer->diamond_height/2


    renderer->tiles[new_id].texture_id = texture_id;
    renderer->tiles[new_id].x = x;
    renderer->tiles[new_id].y = y;
    renderer->tiles[new_id].w = w == 0 ? renderer->graphics.textures[texture_id].width : w;
    renderer->tiles[new_id].h = h == 0 ? renderer->graphics.textures[texture_id].height : h;
    renderer->tiles[new_id].offset_x = - (renderer->tiles[new_id].w/2);
    renderer->tiles[new_id].offset_y = (renderer->diamond_height/2) - (renderer->tiles[new_id].h/2);

    if (renderer->tile_texture_count[texture_id]++ == 0) {
        renderer->tile_texture_offset[texture_id] = new_id;
    }

    return new_id;
}

TileId add_tile_with_centerbottom_offset(RendererIsometric *renderer, TextureId texture_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                            int8_t offset_x_from_center, int8_t offset_y_from_bottom) {
    TileId new_id = renderer->tile_count++;
    assert(new_id >= 0);
    assert(new_id < renderer->tile_count);
    assert(new_id < TILE_MAX);

    renderer->tiles[new_id].texture_id = texture_id;
    renderer->tiles[new_id].x = x;
    renderer->tiles[new_id].y = y;
    renderer->tiles[new_id].w = w == 0 ? renderer->graphics.textures[texture_id].width : w;
    renderer->tiles[new_id].h = h == 0 ? renderer->graphics.textures[texture_id].height : h;
    renderer->tiles[new_id].offset_x = - ((renderer->tiles[new_id].w/2)+offset_x_from_center);
    renderer->tiles[new_id].offset_y = (renderer->diamond_height/2) - offset_y_from_bottom;

    if (renderer->tile_texture_count[texture_id]++ == 0) {
        renderer->tile_texture_offset[texture_id] = new_id;
    }

    return new_id;
}

TileId add_tile_with_offset(RendererIsometric *renderer, TextureId texture_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                            int8_t offset_x, int8_t offset_y) {
    TileId new_id = renderer->tile_count++;
    assert(new_id >= 0);
    assert(new_id < renderer->tile_count);
    assert(new_id < TILE_MAX);

    renderer->tiles[new_id].texture_id = texture_id;
    renderer->tiles[new_id].x = x;
    renderer->tiles[new_id].y = y;
    renderer->tiles[new_id].w = w == 0 ? renderer->graphics.textures[texture_id].width : w;
    renderer->tiles[new_id].h = h == 0 ? renderer->graphics.textures[texture_id].height : h;
    renderer->tiles[new_id].offset_x = offset_x;
    renderer->tiles[new_id].offset_y = offset_y;

    if (renderer->tile_texture_count[texture_id]++ == 0) {
        renderer->tile_texture_offset[texture_id] = new_id;
    }

    return new_id;
}


//void pos_isometric_to_screen(RendererIsometric*, const t_vec3 src_iso_pos, t_ivec2 dest_screen_pos);
//void pos_screen_to_isometric(RendererIsometric*, const t_ivec2 source, t_vec2 dest_iso_pos); //this cannot determine z!

int test_pos_conv(struct RendererIsometric* renderer) {
    int pass = 1;
    for (uint8_t x = 0; x < 10; x++) {
        for (uint8_t y = 0; y < 10; y++) {
            t_vec3 src_iso_pos = { (float)x, (float)y, 0.0f };
            t_ivec2 dest_screen_pos = { 0 };
            pos_isometric_to_screen(renderer, src_iso_pos, dest_screen_pos);

            t_vec2 recon_iso_pos = { 0.0f };
            pos_screen_to_isometric(renderer, dest_screen_pos, recon_iso_pos);

            if(recon_iso_pos[0] != src_iso_pos[0] || recon_iso_pos[1] != src_iso_pos[1]) {
                printf("iso{%f, %f, %f} -> screen{%d, %d} -> iso{%f, %f}\n",
                       src_iso_pos[0], src_iso_pos[1], src_iso_pos[2],
                       dest_screen_pos[0], dest_screen_pos[1],
                       recon_iso_pos[0], recon_iso_pos[1]);
                pass = 0;
            }
        }
    }
    fatal_if(!pass, "Failed isometric screen conversion test");
    return pass;
}

#include "hardcoded_iso_art.c_snippet"

void renderer_isometric_clear(RendererIsometric* renderer) {
    graphics_clear(&renderer->graphics);
}

void renderer_isometric_free(RendererIsometric* renderer) {
    graphics_free(&renderer->graphics);
}

void renderer_isometric_init(RendererIsometric* renderer, int width, int height) {
    graphics_init(&renderer->graphics, width, height);

    glmc_vec3_set(renderer->screen_center_pos, 20.0f, 20.0f, 0.0f);
    glmc_ivec2_set(renderer->screen_translate, 0, 0);
    renderer->tile_count = 0;
    memset(renderer->tiles, 0, sizeof(Tile) * TILE_MAX);
    memset(renderer->tile_texture_offset, 0, sizeof(TileId) * TEXTURE__COUNT);
    memset(renderer->tile_texture_count, 0, sizeof(size_t) * TEXTURE__COUNT);

    renderer->diamond_width = 128;
    renderer->tile_layer_height = 8; //1 tile Z
    renderer->diamond_height = 64;

    add_hardcoded_iso_art(renderer);

    renderer->screen_translate[0] = 0;
    renderer->screen_translate[1] = 0;
//    test_pos_conv(renderer);
}

void pos_isometric_to_screen(RendererIsometric* renderer, const t_vec3 src_iso_pos, t_ivec2 dest_screen_pos) {
    float tilex = src_iso_pos[0];
    float tiley = src_iso_pos[1];
    float tilez = -src_iso_pos[2]; //screen Y axis and iso Y axis are reversed!
    dest_screen_pos[0] = (int) round(((tilex + tiley) * renderer->diamond_width) / 2.0);
    dest_screen_pos[1] = (int) round(((tiley - tilex) * renderer->diamond_height) / 2.0 - (renderer->tile_layer_height * tilez));
    dest_screen_pos[1] = renderer->graphics.height - dest_screen_pos[1];

    dest_screen_pos[0] = dest_screen_pos[0] + renderer->screen_translate[0];
    dest_screen_pos[1] = dest_screen_pos[1] + renderer->screen_translate[1];
}

void pos_isometric_flatten(RendererIsometric* renderer, const t_vec3 src_iso_pos, t_vec2 dest_iso_pos) {
    //Makes the Z component of the isometric position 0.
    //This works because on screen, each X,Y,Z pos has a corresponding X,Y,0 position that has the same screen position.
    //The result of this is only really useful as an intermediary helper when trying to figure out which screen position matches which iso position.

    //Easiest implementation: to screen, then back again:
    t_ivec2 screen_pos;
    pos_isometric_to_screen(renderer, src_iso_pos, screen_pos);
    t_vec3 res3;
    pos_screen_to_isometric(renderer, screen_pos, res3);
    dest_iso_pos[0] = res3[0];
    dest_iso_pos[1] = res3[1];
}

void pos_screen_to_isometric(RendererIsometric* renderer, const t_ivec2 orig_src_screen_pos, t_vec2 dest_iso_pos) {
    t_ivec2 src_screen_pos = {
            orig_src_screen_pos[0] - renderer->screen_translate[0],
            renderer->graphics.height - (orig_src_screen_pos[1] - renderer->screen_translate[1])
    };
    dest_iso_pos[0] = ((float)src_screen_pos[0] / (float)renderer->diamond_width) - ((float)src_screen_pos[1] / (float)renderer->diamond_height);
    dest_iso_pos[1] = ((float)src_screen_pos[0] / (float)renderer->diamond_width) + ((float)src_screen_pos[1] / (float)renderer->diamond_height);
}

void renderer_isometric_begin_draw(RendererIsometric* renderer) {
    graphics_begin_draw(&renderer->graphics);

    renderer->screen_translate[0] = 0;
    renderer->screen_translate[1] = 0;
    t_ivec2 new_screen_translate = {0, 0};
    pos_isometric_to_screen(renderer, renderer->screen_center_pos, new_screen_translate);
    renderer->screen_translate[0] = (renderer->graphics.width/2) - new_screen_translate[0];
    renderer->screen_translate[1] = (renderer->graphics.height/2) - new_screen_translate[1];
//    test_pos_conv(renderer);
}

void renderer_isometric_end_draw(RendererIsometric* renderer) {
    graphics_end_draw(&renderer->graphics);
}

void pos_isometric_to_3d(RendererIsometric* renderer, t_vec3 source, t_vec3 dest) {
    //isometric 3D pos and 3D pos are the same at the moment
    glmc_assign_vec3(dest, source);
}

void pos_3d_to_isometric(RendererIsometric* renderer, t_vec3 source, t_vec3 dest) {
    //isometric 3D pos and 3D pos are the same at the moment
    glmc_assign_vec3(dest, source);
}

void renderer_isometric_draw_tile(RendererIsometric* renderer, TileId tile_id, t_vec3 iso_pos) {
    t_ivec2 target_screen_pos;
    pos_isometric_to_screen(renderer, iso_pos, target_screen_pos);
    assert(tile_id >= 0);
    assert(tile_id < renderer->tile_count);
    Tile* tile = &renderer->tiles[tile_id];
    int y_diff = tile->h - renderer->diamond_height;

    assert(tile->x >= 0);
    assert(tile->y >= 0);
    assert(tile->w >= 0);
    assert(tile->h >= 0);
    assert(tile->x < renderer->graphics.textures[tile->texture_id].width);
    assert(tile->y < renderer->graphics.textures[tile->texture_id].height);
    assert(tile->x + tile->w <= renderer->graphics.textures[tile->texture_id].width);
    assert(tile->y + tile->h <= renderer->graphics.textures[tile->texture_id].height);

//    printf("Rendering tile %u as texture %u offset %d,%d  w=%d h=%d\n",
//           art_id, tile->texture_id, tile->x, tile->y, tile->w, tile->h);
    texture_draw(&renderer->graphics.textures[tile->texture_id],
                 target_screen_pos[0] + tile->offset_x,
                 target_screen_pos[1] - (tile->offset_y + y_diff),
                 tile->x, tile->y, tile->w, tile->h
    );
}

void renderer_isometric_grid(RendererIsometric* renderer, int center, float z, uint8_t r, uint8_t g, uint8_t b) {
    int x_diff = 0;
    int y_diff = renderer->diamond_height/2;

    if (center) {
        x_diff = renderer->diamond_width/2;
        y_diff = renderer->diamond_height/2;
    }

    SDL_SetRenderDrawColor(renderer->graphics.sdl_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    for (int x = 0; x < 2; x++) {
        t_vec3 iso_pos_a = {1.0f * x, 0.0f, z};
        t_vec3 iso_pos_b = {1.0f * x, 10.0f, z};
        t_ivec2 target_screen_pos_a;
        t_ivec2 target_screen_pos_b;
        pos_isometric_to_screen(renderer, iso_pos_a, target_screen_pos_a);
        pos_isometric_to_screen(renderer, iso_pos_b, target_screen_pos_b);

        SDL_RenderDrawLine(renderer->graphics.sdl_renderer,
                           target_screen_pos_a[0] + x_diff, target_screen_pos_a[1] + y_diff,
                           target_screen_pos_b[0] + x_diff, target_screen_pos_b[1] + y_diff);
    }
    for (int y = 0; y < 2; y++) {
        t_vec3 iso_pos_a = {0.0f, 1.0f * y, z};
        t_vec3 iso_pos_b = {10.0f, 1.0f * y, z};
        t_ivec2 target_screen_pos_a;
        t_ivec2 target_screen_pos_b;
        pos_isometric_to_screen(renderer, iso_pos_a, target_screen_pos_a);
        pos_isometric_to_screen(renderer, iso_pos_b, target_screen_pos_b);

        SDL_RenderDrawLine(renderer->graphics.sdl_renderer,
                           target_screen_pos_a[0] + x_diff, target_screen_pos_a[1] + y_diff,
                           target_screen_pos_b[0] + x_diff, target_screen_pos_b[1] + y_diff);
    }
}

void renderer_isometric_tile_border(RendererIsometric* renderer, t_vec3 iso_pos, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int x_diff = 0;
    int y_diff = renderer->diamond_height/2;

    SDL_Point points[5];

    SDL_SetRenderDrawColor(renderer->graphics.sdl_renderer, r, g, b, a);
    t_vec3 iso_pos_a = {iso_pos[0], iso_pos[1], iso_pos[2]};
    t_vec3 iso_pos_b = {iso_pos[0]+1.0f, iso_pos[1], iso_pos[2]};
    t_vec3 iso_pos_c = {iso_pos[0]+1.0f, iso_pos[1]+1.0f, iso_pos[2]};
    t_vec3 iso_pos_d = {iso_pos[0], iso_pos[1]+1.0f, iso_pos[2]};

    t_ivec2 target_screen_pos[4];
    pos_isometric_to_screen(renderer, iso_pos_a, target_screen_pos[0]);
    pos_isometric_to_screen(renderer, iso_pos_b, target_screen_pos[1]);
    pos_isometric_to_screen(renderer, iso_pos_c, target_screen_pos[2]);
    pos_isometric_to_screen(renderer, iso_pos_d, target_screen_pos[3]);

    for (int i = 0; i < 4; i++) {
        points[i].x = target_screen_pos[i][0] + x_diff;
        points[i].y = target_screen_pos[i][1] + y_diff;
    }
    points[4].x = target_screen_pos[0][0] + x_diff;
    points[4].y = target_screen_pos[0][1] + y_diff;

    SDL_RenderDrawLines(renderer->graphics.sdl_renderer, points, 5);
}

void renderer_isometric_rect(RendererIsometric* renderer, t_vec3 iso_pos0, t_vec3 iso_pos1, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int x_diff = 0;
    int y_diff = renderer->diamond_height/2;

    SDL_Point points[5];

    SDL_SetRenderDrawColor(renderer->graphics.sdl_renderer, r, g, b, a);
    t_vec3 iso_pos_a = {iso_pos0[0], iso_pos0[1], iso_pos0[2]};
    t_vec3 iso_pos_b = {iso_pos1[0], iso_pos0[1], iso_pos0[2]};
    t_vec3 iso_pos_c = {iso_pos1[0], iso_pos1[1], iso_pos0[2]};
    t_vec3 iso_pos_d = {iso_pos0[0], iso_pos1[1], iso_pos0[2]};

    t_ivec2 target_screen_pos[4];
    pos_isometric_to_screen(renderer, iso_pos_a, target_screen_pos[0]);
    pos_isometric_to_screen(renderer, iso_pos_b, target_screen_pos[1]);
    pos_isometric_to_screen(renderer, iso_pos_c, target_screen_pos[2]);
    pos_isometric_to_screen(renderer, iso_pos_d, target_screen_pos[3]);

    for (int i = 0; i < 4; i++) {
        points[i].x = target_screen_pos[i][0] + x_diff;
        points[i].y = target_screen_pos[i][1] + y_diff;
    }
    points[4].x = target_screen_pos[0][0] + x_diff;
    points[4].y = target_screen_pos[0][1] + y_diff;

    SDL_RenderDrawLines(renderer->graphics.sdl_renderer, points, 5);
}
