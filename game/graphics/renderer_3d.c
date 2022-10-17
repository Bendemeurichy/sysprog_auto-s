#include "renderer_3d.h"

#include "hardcoded_3d_art.c_snippet"

#include <assert.h>
#include <glmc.h>

void renderer_3d_clear(Renderer3D* renderer) {
    renderer_isometric_clear(&renderer->renderer_isometric);
}

void renderer_3d_free(Renderer3D* renderer) {
    renderer_isometric_free(&renderer->renderer_isometric);
}

#define NOT_FACING N

void renderer_3d_init(Renderer3D* renderer, int width, int height) {
    renderer_isometric_init(&renderer->renderer_isometric, width, height);

    renderer->art_ground_count = 0;
    renderer->art_various_count = 0;
    renderer->art_building_count = 0;
    renderer->art_colored_count = 0;
    renderer->art_car_count = 0;

    renderer->rects_count = 0;
    renderer->buffer_objects_count = 0;

    renderer->art_ground_id_unknown = 0;
    renderer->art_various_id_unknown = 0;

    for (ArtBuildingId id = 0; id < ART_BUILDING_MAX; id++) {
        ArtBuilding3D* art = &renderer->art_building[id];
        art->type = ART_BUILDING__COUNT+1;
    }

    add_hardcoded_3d_art(renderer);

    //hardcode the HUD tileIds
    assert(renderer->renderer_isometric.tile_texture_count[TEXTURE_HUD] == 14);
    renderer->tile_id_hud_gameover = renderer->renderer_isometric.tile_texture_offset[TEXTURE_HUD] + 0;
    renderer->tile_id_hud_gamewin = renderer->renderer_isometric.tile_texture_offset[TEXTURE_HUD] + 11;
    renderer->tile_id_hud_actionbar = renderer->renderer_isometric.tile_texture_offset[TEXTURE_HUD] + 12;
    renderer->tile_id_hud_actionbar_highlight = renderer->renderer_isometric.tile_texture_offset[TEXTURE_HUD] + 13;
    for (int i = 0; i < 10; i++) {
        renderer->tile_id_hud_number[i] = renderer->renderer_isometric.tile_texture_offset[TEXTURE_HUD] + 1 + i;
    }

    assert(renderer->art_ground_count > 0);
    assert(renderer->art_building_count > 0);
    assert(renderer->art_car_count > 0);

    renderer->art_various_id_tile_sub_highlight = renderer_3d_various_lookup(renderer, ART_TILE_SUB_HIGHLIGHT, 0);
    renderer->art_various_id_tile_sub_highlight_45 = renderer_3d_various_lookup(renderer, ART_TILE_SUB_HIGHLIGHT_45, 0);
    renderer->art_various_id_tile_sub_highlight_center = renderer_3d_various_lookup(renderer, ART_TILE_SUB_HIGHLIGHT_CENTER, 0);
    renderer->art_various_id_tile_highlight = renderer_3d_various_lookup(renderer, ART_TILE_HIGHLIGHT, 0);
    renderer->art_various_id_unknown = renderer_3d_various_lookup(renderer, ART_UNKNOWN, 0);
    assert(renderer->art_various_id_tile_sub_highlight != ART_VARIOUS_ID_NONE);
    assert(renderer->art_various_id_tile_sub_highlight_45 != ART_VARIOUS_ID_NONE);
    assert(renderer->art_various_id_tile_sub_highlight_center != ART_VARIOUS_ID_NONE);
    assert(renderer->art_various_id_tile_highlight != ART_VARIOUS_ID_NONE);
    assert(renderer->art_various_id_unknown != ART_VARIOUS_ID_NONE);

    renderer->art_colored_id_highlight = renderer_3d_colored_lookup(renderer, ART_HIGHLIGHT, 0);

    //last ArtGroundId: fake one that will be used for UNKNOWN (always last TileId)
    {
        renderer->art_ground_id_unknown = add_special_art_ground(renderer, TEXTURE_EXTRA, 16);
    }

    //hardcode the base 1x1 and 3x3 ArtIds
    ArtGroundId unknown_ground_id = add_special_art_ground(renderer, TEXTURE_BASE, 9);
    for (Material mat = 0; mat < ART__COUNT; mat++) {
        renderer->art_ground_id_base_1x1[mat] = unknown_ground_id;
        renderer->art_ground_id_base_3x3[mat] = unknown_ground_id;
    }

    renderer->art_ground_id_base_3x3[ART_ROAD] = add_special_art_ground(renderer, TEXTURE_BASE, 0);
    renderer->art_ground_id_base_3x3[ART_GRASS] = add_special_art_ground(renderer, TEXTURE_BASE, 1);
    renderer->art_ground_id_base_3x3[ART_DIRT] = add_special_art_ground(renderer, TEXTURE_BASE, 2);
    renderer->art_ground_id_base_3x3[ART_WATER] = add_special_art_ground(renderer, TEXTURE_BASE, 3);
//    renderer->art_ground_id_base_1x1[ART_ROAD] = add_special_art_ground(renderer, TEXTURE_BASE, 4);
    renderer->art_ground_id_base_1x1[ART_ROAD] = add_special_art_ground(renderer, TEXTURE_BASE_EDGE, 0);
//    renderer->art_ground_id_base_1x1[ART_GRASS] = add_special_art_ground(renderer, TEXTURE_BASE, 5);
    renderer->art_ground_id_base_1x1[ART_GRASS] = add_special_art_ground(renderer, TEXTURE_BASE_EDGE, 1);
//    renderer->art_ground_id_base_1x1[ART_DIRT] = add_special_art_ground(renderer, TEXTURE_BASE, 6);
    renderer->art_ground_id_base_1x1[ART_DIRT] = add_special_art_ground(renderer, TEXTURE_BASE_EDGE, 2);
//    renderer->art_ground_id_base_1x1[ART_WATER] = add_special_art_ground(renderer, TEXTURE_BASE, 7);
    renderer->art_ground_id_base_1x1[ART_WATER] = add_special_art_ground(renderer, TEXTURE_BASE_EDGE, 3);
//    renderer->art_ground_id_base_1x1[ART_EDGE] = add_special_art_ground(renderer, TEXTURE_BASE, 8);
    renderer->art_ground_id_base_1x1[ART_EDGE] = add_special_art_ground(renderer, TEXTURE_BASE_EDGE, 4);
}

void renderer_3d_begin_draw(Renderer3D *renderer) {
    //clear the buffered draws
    renderer_isometric_begin_draw(&renderer->renderer_isometric);

    renderer->buffer_objects_count = 0;
    renderer->rects_count = 0;
    renderer->buffer_hud_count = 0;
    for (int x = 0; x < BUFFER_DIM_Y; x++) {
        for (int y = 0; y < BUFFER_DIM_Y; y++) {
            assert(x >= 0);
            assert(y >= 0);
            assert(x < BUFFER_DIM_X);
            assert(y < BUFFER_DIM_Y);
            renderer->buffer_ground_by_pos[x][y].art_id = ART_GROUND_ID_NONE;
            renderer->buffer_object_by_pos[x][y]= BUFFER_OBJECT_NONE;
        }
    }
}


void actual_renderer_3d_ground_draw(Renderer3D* renderer, ArtGroundId art_id, t_vec3 position) {
    assert(art_id >= 0);
    assert(art_id < renderer->art_ground_count);

    t_vec3 tile_pos = {0};
    pos_3d_to_isometric(&renderer->renderer_isometric, position, tile_pos);
    ArtGround3D* art = &renderer->art_ground[art_id];
//    printf("Rendering art_ground %u as tile %u at %f,%f,%f\n",
//           art_id, art_ground->art_id, tile_pos[0], tile_pos[1], tile_pos[2]);
    TileId tile_id = art->tile_id;
    renderer_isometric_draw_tile(&renderer->renderer_isometric, tile_id, tile_pos);
}


void actual_renderer_3d_isometric_draw(Renderer3D* renderer, TileId isometric_tile_id, t_vec3 position) {
    assert(isometric_tile_id >= 0);
    assert(isometric_tile_id < renderer->renderer_isometric.tile_count);

    t_vec3 tile_pos = {0};
    pos_3d_to_isometric(&renderer->renderer_isometric, position, tile_pos);
    renderer_isometric_draw_tile(&renderer->renderer_isometric, isometric_tile_id, tile_pos);
}


void actual_renderer_3d_rect_draw(Renderer3D* renderer, t_vec3 pos0, t_vec3 pos1, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    t_vec3 tile_pos0 = {0};
    t_vec3 tile_pos1 = {0};
    pos_3d_to_isometric(&renderer->renderer_isometric, pos0, tile_pos0);
    pos_3d_to_isometric(&renderer->renderer_isometric, pos1, tile_pos1);

    renderer_isometric_rect(&renderer->renderer_isometric, tile_pos0, tile_pos1, r, g, b, a);
}

void actual_renderer_3d_hud_draw(Renderer3D* renderer, BufferedHUD* hud) {
    Tile* tile = &renderer->renderer_isometric.tiles[hud->tile_id];
//    int y_diff = tile->h - renderer->renderer_isometric.diamond_height;

    texture_draw(&renderer->renderer_isometric.graphics.textures[TEXTURE_HUD],
                 hud->position[0], hud->position[1],
                 tile->x, tile->y, tile->w, tile->h
    );
}

void renderer_3d_end_draw(Renderer3D* renderer) {
    //do the actual buffered draws
//    for (int x = BUFFER_MIN_X; x <= BUFFER_MAX_X; x++) {  //wrong!
//        for (int y = BUFFER_MAX_Y; y >= BUFFER_MIN_Y; y--) {  //wrong!
    for (int x = 0; x < BUFFER_DIM_X; x++) {
        for (int y = BUFFER_DIM_Y-1; y >= 0; y--) {
            assert(x >= 0);
            assert(y >= 0);
            assert(x < BUFFER_DIM_X);
            assert(y < BUFFER_DIM_Y);

            if (renderer->buffer_ground_by_pos[x][y].art_id != ART_GROUND_ID_NONE) {
                actual_renderer_3d_ground_draw(
                        renderer,
                        renderer->buffer_ground_by_pos[x][y].art_id,
                        renderer->buffer_ground_by_pos[x][y].position
                );
            }
//            //hack for 3x3 iso ground tiles
//            if (x > 2 && x % 3 == 2 && y % 3 == 0 &&
//                renderer->buffer_ground_by_pos[x][y].art_id == ART_GROUND_ID_NONE &&
//                renderer->buffer_ground_by_pos[x - 2][y].art_id != ART_GROUND_ID_NONE) {
//                actual_renderer_3d_ground_draw(
//                        renderer,
//                        renderer->buffer_ground_by_pos[x - 2][y].art_id,
//                        renderer->buffer_ground_by_pos[x - 2][y].position
//                );
//            }

            BufferedObjectId render_object_id = renderer->buffer_object_by_pos[x][y];
            while(render_object_id != BUFFER_OBJECT_NONE) {
                BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
                actual_renderer_3d_isometric_draw(renderer, buffered_object->isometric_tile_id, buffered_object->position);
                render_object_id = buffered_object->next_object;
            }
        }
    }

//    printf("renderer->rects_count=%lu\n", renderer->rects_count);
    for (size_t i = 0; i < renderer->rects_count; i++) {
        actual_renderer_3d_rect_draw(
                renderer, renderer->rects[i].pos0, renderer->rects[i].pos1,
                renderer->rects[i].r, renderer->rects[i].g, renderer->rects[i].b, renderer->rects[i].a
        );
    }

//    //render lines of isometric grid
//    renderer_isometric_grid(&renderer->renderer_isometric, 0, 4.0f, 255, 0, 0);
//    renderer_isometric_grid(&renderer->renderer_isometric, 0, 0.0f, 0, 255, 0);
//    renderer_isometric_grid(&renderer->renderer_isometric, 1, 4.0f, 0, 0, 255);

//    //render tile outline of isometric grid
//    t_vec3 iso_pos = {2.0f, 2.0f, 4.0f};
//    renderer_isometric_tile_border(&renderer->renderer_isometric, iso_pos, 255, 255, 0, 255);

    for (size_t i = 0; i < renderer->buffer_hud_count; i++) {
        actual_renderer_3d_hud_draw(renderer, &renderer->buffer_hud[i]);
    }

    renderer_isometric_end_draw(&renderer->renderer_isometric);
}


ArtGroundId renderer_3d_ground_lookup(Renderer3D* renderer, ArtGround3D* art_3d) {
    for (ArtGroundId art_id = 0; art_id < renderer->art_ground_count; art_id++) {
        ArtGround3D* cur = &renderer->art_ground[art_id];
        if (cur->alt_art_index != art_3d->alt_art_index) {
            continue;
        }
        int matches = 1;
        for (int i = 0; i < ART_POS__COUNT; i++) {
            if (cur->materials[i] != art_3d->materials[i]) {
                matches = 0;
                break;
            }
            if (cur->height[i] != art_3d->height[i]) {
                matches = 0;
                break;
            }
        }
        if (matches) {
            return art_id;
        }
    }
    return ART_GROUND_ID_NONE;
}

ArtColoredId renderer_3d_colored_lookup(Renderer3D* renderer, ArtColoredType type, uint8_t art_alt_index) {
    for (ArtColoredId art_id = 0; art_id < renderer->art_colored_count; art_id++) {
        ArtColored3D* cur = &renderer->art_colored[art_id];
        if (cur->type != type) {
            continue;
        }
        if (cur->alt_art_index != art_alt_index) {
            continue;
        }
        return art_id;
    }
    return ART_COLORED_ID_NONE;
}

ArtVariousId renderer_3d_various_lookup(Renderer3D* renderer, ArtVariousType type, uint8_t art_alt_index) {
    for (ArtVariousId art_id = 0; art_id < renderer->art_various_count; art_id++) {
        ArtVarious3D* cur = &renderer->art_various[art_id];
        if (cur->type != type) {
            continue;
        }
        if (cur->alt_art_index != art_alt_index) {
            continue;
        }
        return art_id;
    }
    return ART_VARIOUS_ID_NONE;
}

ArtBuildingId renderer_3d_building_lookup(Renderer3D* renderer, ArtBuildingType type,
                                          BuildingRoofType roof_type, BuildingColor wall_color,
                                          BuildingWindow window_type, BuildingExtra extra, BuildingColor extra_color,
                                          uint8_t art_alt_index) {
    if (roof_type != -1) {
        assert(roof_type >= 0);
        assert(roof_type < COUNT_ROOF_TYPE);
    }
    if (wall_color != -1) {
        assert(wall_color >= 0);
        assert(wall_color < COUNT_BUILDING_COLOR);
    }
    if (window_type != -1) {
        assert(window_type >= 0);
        assert(window_type < COUNT_BUILDING_WINDOW);
    }

    assert(renderer->art_building_count > 0);
    size_t rand_start = rand() % renderer->art_building_count;
//    size_t rand_start = 0;
    for (ArtBuildingId i = 0; i < renderer->art_building_count; i++) {
        ArtBuildingId art_id = (i + rand_start) % renderer->art_building_count;
        assert(art_id < renderer->art_building_count);
        ArtBuilding3D* cur = &renderer->art_building[art_id];
        assert(cur->type < ART_BUILDING__COUNT);
        if (cur->type != type) {
            continue;
        }
        if (art_alt_index != 0xFF && cur->alt_art_index != art_alt_index) {
            continue;
        }
        if (wall_color != -1 && cur->wall_color != wall_color) {
            continue;
        }
        if (cur->type == ART_BUILDING_ROOF) {
            if (roof_type != -1 && cur->roof_type != roof_type) {
                continue;
            }
        }
        if (cur->type == ART_BUILDING_GROUND_FLOOR) {
            if (window_type != -1 && cur->window_type != window_type) {
                continue;
            }
            if (extra != -1 && cur->extra != extra) {
                continue;
            }
            if (extra_color != -1 && cur->extra_color != extra_color) {
                continue;
            }
        }
        if (cur->type == ART_BUILDING_STORY) {
            if (window_type != -1 && cur->window_type != window_type) {
                continue;
            }
        }
        return art_id;
    }
    return ART_BUILDING_ID_NONE;
}

void renderer_3d_ground_draw(Renderer3D* renderer, ArtGroundId art_id, t_vec3 position) {
    assert(art_id != ART_GROUND_ID_NONE);
    //this buffers the draw, so it can be done in the correct order
    int x = (int) ceilf(position[0]*3.0f);
    int y = (int) floorf(position[1]*3.0f);
    assert(x >= BUFFER_MIN_X);
    assert(x <= BUFFER_MAX_X);
    assert(y >= BUFFER_MIN_Y);
    assert(y <= BUFFER_MAX_Y);
    x -= BUFFER_MIN_X;
    y -= BUFFER_MIN_Y;
    assert(x >= 0);
    assert(x < BUFFER_DIM_X);
    assert(y >= 0);
    assert(y < BUFFER_DIM_Y);
    glmc_assign_vec3(renderer->buffer_ground_by_pos[x][y].position, position);
    assert(art_id >= 0);
    assert(art_id < renderer->art_ground_count);
    renderer->buffer_ground_by_pos[x][y].art_id = art_id;
}

void add_buffered_object(Renderer3D* renderer, BufferedObjectId render_object_id, t_vec3 position, int tweak_offset) {
    //Complication: if the tile has an offset, we need to substract it from x/y.
    //              this would require some calculations, special cases etc. Instead, we cheat using "tweak_offset" (a hack)
    //              this hack uses the simple fact that we need this for cars, but not for buildings
    int x = (int) ((position[0] + (tweak_offset * 0.2f))*3.0f);
    int y = (int) ((position[1] - (tweak_offset * 0.2f))*3.0f);

    if (x < BUFFER_MIN_X) {
        x = BUFFER_MIN_X;
    }
    if (y < BUFFER_MIN_X) {
        y = BUFFER_MIN_X;
    }

    assert(x >= BUFFER_MIN_X);
    assert(x <= BUFFER_MAX_X);
    assert(y >= BUFFER_MIN_Y);
    assert(y <= BUFFER_MAX_Y);
    x -= BUFFER_MIN_X;
    y -= BUFFER_MIN_Y;
    assert(x >= 0);
    assert(x < BUFFER_DIM_X);
    assert(y >= 0);
    assert(y < BUFFER_DIM_Y);

    //simple add would be:
//    buffered_object->next_object = renderer->buffer_object_by_pos[x][y];
//    renderer->buffer_object_by_pos[x][y] = render_object_id;
    //(but simple add is not good enough, it needs to be sorted by z.)

    //sorted by Z add
    //Fun exercise: what is the average/worst complexity of this operation?
    //   (Tip: this is NOT "insertion sort on an array". This is "insert sorted into a linked list".)
    int safety = 20;

    BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
    int8_t z_priority = buffered_object->z_priority;

    BufferedObjectId cur_object_id = renderer->buffer_object_by_pos[x][y];
    BufferedObject* prev = NULL;
    BufferedObject* cur = cur_object_id == BUFFER_OBJECT_NONE ? NULL : &renderer->buffer_objects[cur_object_id];
    while (cur != NULL && (cur->position[2] < position[2] || cur->z_priority < z_priority)) {
        prev = cur;

        cur_object_id = cur->next_object;
        cur = cur_object_id == BUFFER_OBJECT_NONE ? NULL : &renderer->buffer_objects[cur_object_id];

        assert(safety-- >= 0);
    }
    assert(cur == NULL || cur->position[2] >= position[2] || cur->z_priority >= z_priority);
    assert(prev == NULL || prev->position[2] < position[2] || prev->z_priority < z_priority);

    assert(buffered_object->isometric_tile_id >= 0);
    assert(buffered_object->isometric_tile_id < renderer->renderer_isometric.tile_count);
    buffered_object->next_object = cur_object_id;
    if (prev == NULL) {
        renderer->buffer_object_by_pos[x][y] = render_object_id;
    } else {
        prev->next_object = render_object_id;
    }
}

void renderer_3d_building_draw(Renderer3D* renderer, ArtBuildingId art_id, Direction dir, t_vec3 position) {
    assert(dir >= 0 && dir < 4);
    assert(renderer->buffer_objects_count <= BUFFER_MAX_OBJECTS);
    assert(art_id != ART_BUILDING_ID_NONE);
    BufferedObjectId render_object_id = renderer->buffer_objects_count++;
    BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
    glmc_assign_vec3(buffered_object->position, position);
    assert(art_id >= 0);
    assert(art_id < renderer->art_building_count);

    ArtBuilding3D* art_obj = &renderer->art_building[art_id];
    buffered_object->z_priority = 0;
    buffered_object->isometric_tile_id = art_obj->tile_id_by_dir[dir];
    assert(renderer->renderer_isometric.tile_texture_offset[TEXTURE_BUILDINGS] > 0);

    assert(buffered_object->isometric_tile_id > 0);
    assert(buffered_object->isometric_tile_id >= renderer->renderer_isometric.tile_texture_offset[TEXTURE_BUILDINGS]);
    assert(buffered_object->isometric_tile_id < renderer->renderer_isometric.tile_texture_offset[TEXTURE_BUILDINGS] + renderer->renderer_isometric.tile_texture_count[TEXTURE_BUILDINGS]);
//    printf("renderer_3d_building_draw %u -> %u\n", art_id, buffered_object->isometric_tile_id);
    add_buffered_object(renderer, render_object_id, position, 0);
}

void renderer_3d_colored_draw(Renderer3D* renderer, ArtColoredId art_id, ArtColor color, t_vec3 position) {
    assert(renderer->buffer_objects_count <= BUFFER_MAX_OBJECTS);
    assert(art_id != ART_COLORED_ID_NONE);
    BufferedObjectId render_object_id = renderer->buffer_objects_count++;
    BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
    glmc_assign_vec3(buffered_object->position, position);
    assert(art_id >= 0);
    assert(art_id < renderer->art_colored_count);

    ArtColored3D* art_obj = &renderer->art_colored[art_id];
    buffered_object->z_priority = art_obj->z_priority;
    buffered_object->isometric_tile_id = art_obj->tile_id_by_color[color];
    add_buffered_object(renderer, render_object_id, position, 2);

//    //special case: pushed button also draws a yellow highlight
//    if ((art_obj->type == ART_BUTTON_SQUARE_PUSHED || art_obj->type == ART_BUTTON_ROUND_PUSHED) &&
//        renderer->art_colored_id_highlight != ART_COLORED_ID_NONE) {
//        render_object_id = renderer->buffer_objects_count++;
//        buffered_object = &renderer->buffer_objects[render_object_id];
//        glmc_assign_vec3(buffered_object->position, position);
//
//        art_obj = &renderer->art_colored[renderer->art_colored_id_highlight];
//        buffered_object->z_priority = art_obj->z_priority;
//        buffered_object->isometric_tile_id = art_obj->tile_id_by_color[ART_COLOR_YELLOW];
//        add_buffered_object(renderer, render_object_id, position, 2);
//    }
}

void renderer_3d_various_draw(Renderer3D* renderer, ArtVariousId art_id, Direction dir, t_vec3 position, int always_on_top) {
    assert(renderer->buffer_objects_count <= BUFFER_MAX_OBJECTS);
    assert(art_id != ART_VARIOUS_ID_NONE);
    BufferedObjectId render_object_id = renderer->buffer_objects_count++;
    BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
    glmc_assign_vec3(buffered_object->position, position);
    assert(art_id >= 0);
    assert(art_id < renderer->art_various_count);

    ArtVarious3D* art_obj = &renderer->art_various[art_id];
    buffered_object->z_priority = art_obj->z_priority;
    buffered_object->isometric_tile_id = art_obj->tile_id_by_dir[dir];
    add_buffered_object(renderer, render_object_id, position, always_on_top ? 10 : 1);
}

void renderer_3d_car_draw(Renderer3D* renderer, ArtCarId art_car_id, t_vec3 position, Direction8 dir, int8_t up_down) {
    //this buffers the draw, so it can be done in the correct order

    assert(renderer->buffer_objects_count <= BUFFER_MAX_OBJECTS);
    assert(art_car_id != ART_CAR_ID_NONE);
    BufferedObjectId render_object_id = renderer->buffer_objects_count++;
    BufferedObject* buffered_object = &renderer->buffer_objects[render_object_id];
    glmc_assign_vec3(buffered_object->position, position);
    assert(art_car_id >= 0);
    assert(art_car_id < renderer->art_car_count);

    ArtCar3D* art_obj = &renderer->art_cars[art_car_id];
    TileId tile_id = -1;
    if (up_down == 0) {
        assert(dir >= 0);
        assert(dir < 8);
        tile_id = art_obj->tile_id_by_dir[dir];
    } else if (up_down == -1) {
        assert(direction8_to_direction(dir) >= 0);
        assert(direction8_to_direction(dir) < 4);
        tile_id = art_obj->tile_id_down_by_dir[direction8_to_direction(dir)];
    } else if (up_down == 1) {
        assert(direction8_to_direction(dir) >= 0);
        assert(direction8_to_direction(dir) < 4);
        tile_id = art_obj->tile_id_up_by_dir[direction8_to_direction(dir)];
    }
    buffered_object->z_priority = 0;
    buffered_object->isometric_tile_id = tile_id;

    add_buffered_object(renderer, render_object_id, position, 5);
}

void renderer_3d_hud_draw(Renderer3D* renderer, TileId hud_tile_id, int x, int y) {
    assert(renderer->buffer_hud_count <= MAX_HUD);
    size_t hud_id = renderer->buffer_hud_count++;
    BufferedHUD* buffered_hud = &renderer->buffer_hud[hud_id];
    buffered_hud->tile_id = hud_tile_id;
    glmc_ivec2_set(buffered_hud->position, x, y);
}

void renderer_3d_rect_draw(Renderer3D* renderer, t_vec3 pos0, t_vec3 pos1, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    assert(renderer->rects_count <= BUFFER_MAX_RECTS);
    uint32_t drivable_index = renderer->rects_count++;
    glmc_assign_vec3(renderer->rects[drivable_index].pos0, pos0);
    glmc_assign_vec3(renderer->rects[drivable_index].pos1, pos1);
    renderer->rects[drivable_index].r = r;
    renderer->rects[drivable_index].g = g;
    renderer->rects[drivable_index].b = b;
    renderer->rects[drivable_index].a = a;
}

void renderer_3d_set_camera(Renderer3D* renderer, t_vec3 center_position) {
    t_vec3 screen_center_pos = {0};
    pos_3d_to_isometric(&renderer->renderer_isometric, center_position, screen_center_pos);
    glmc_assign_vec3(renderer->renderer_isometric.screen_center_pos, screen_center_pos);
}





ArtGroundId add_special_art_ground(Renderer3D* renderer, TextureId texture_id, TileId tile_offset_in_texture) {
    Material materials[ART_POS__COUNT] = {ART_FOAM};
    uint8_t art_height[ART_POS__COUNT] = {1};
    ArtGroundId ground_id = add_art_ground(renderer, texture_id, tile_offset_in_texture, 255, materials, art_height);
    printf("Special ground ID: %u\n", ground_id);
    return ground_id;
}

ArtGroundId add_art_ground(Renderer3D* renderer,
                           TextureId texture_id, TileId tile_offset_in_texture, uint8_t alt_art_index,
                           Material *materials,
                           uint8_t *height) {
    assert(texture_id > 0);
    assert(texture_id < TEXTURE__COUNT);
    TileId tile_id = renderer->renderer_isometric.tile_texture_offset[texture_id] + tile_offset_in_texture;

    ArtGroundId art_id = renderer->art_ground_count++;
    assert(art_id >= 0);
    assert(art_id < ART_GROUND_MAX);
    ArtGround3D* art = &renderer->art_ground[art_id];

    art->tile_id = tile_id;
    art->alt_art_index = alt_art_index;
//    memcpy(art->materials, materials, sizeof(Material) * ART_POS__COUNT);
//    memcpy(art->height, height, sizeof(uint8_t) * ART_POS__COUNT);

    for (int i = 0; i < ART_POS__COUNT; i++) {
        art->materials[i] = materials[i];
        art->height[i] = height[i];
    }
    art->min_ground_height = art->height[0];
    for (int i = 0; i < ART_POS__COUNT; i++) {
        if (art->height[i] < art->min_ground_height) {
            art->min_ground_height = art->height[i];
        }
    }

    return art_id;
}

const char* material_to_string(Material material) {
    switch (material) {
        case ART_DIRT: return "ART_DIRT";
        case ART_GRASS: return "ART_GRASS";
        case ART_WATER: return "ART_WATER";
        case ART_PAVED: return "ART_PAVED";
        case ART_ROAD: return "ART_ROAD";
        case ART_SAND: return "ART_SAND";
        case ART_FOAM: return "ART_FOAM";
        case ART_EDGE: return "ART_EDGE";
        default: return "UNKNOWN";
    }
}
