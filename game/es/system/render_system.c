#include "render_system.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

void render_static_info_init(RenderStaticInfo* render, Renderer3D* renderer) {
    render->renderer = renderer;
    render->last_fps_starttime = 0;
    render->current_fps_count = 0;
}

RenderStaticInfo *render_static_info_alloc(Renderer3D* renderer) {
    RenderStaticInfo* res = calloc(1, sizeof(RenderStaticInfo));
    render_static_info_init(res, renderer);
    return res;
}

void render_static_info_free(RenderStaticInfo* render) {
}

static void set_camera(RenderStaticInfo* render, Engine* engine) {
    CameraComponent* cam = search_first_component(engine, COMP_CAMERA);
    assert(cam != NULL);
    if (cam != NULL) {
        renderer_3d_set_camera(render->renderer, cam->current);
    } else {
        t_vec3 center = {20.0f, 20.0f, 0.0f};
        renderer_3d_set_camera(render->renderer, center);
    }
}

int get_fps(RenderStaticInfo* render, Engine* engine) {
    uint64_t now = ((TimeComponent*)search_first_component(engine, COMP_TIME))->time;
    if (render->last_fps_starttime == 0) {
        render->last_fps_starttime = now;
        return 0;
    }
    uint64_t diff_ms = now - render->last_fps_starttime;
    render->current_fps_count++;

    if (diff_ms >= 1000) {
        render->last_fps_count = (int) round(render->current_fps_count * 1000.0 / diff_ms);
        render->last_fps_starttime = now;
        render->current_fps_count = 0;
    }

    return render->last_fps_count;
}

#define HUD_LEVEL_Y_POS (10u)
#define HUD_LEVEL_X_POS (10u)
#define HUD_NUMBER_WIDTH (30u)
#define HUD_NUMBER_HEIGHT (56u)
#define HUD_FPS_X_POS (10u)
#define HUD_FPS_Y_POS (10u)
#define HUD_GAMEOVER_W (436u)
#define HUD_GAMEOVER_H (211u)
#define HUD_GAMEWIN_W (324u)
#define HUD_GAMEWIN_H (324u)
#define HUD_ACTIONBAR_W (1024u)
#define HUD_ACTIONBAR_H (102u)
#define HUD_ACTIONBAR_HL_H (71u)
#define HUD_ACTIONBAR_HL_W (71u)
#define HUD_ACTIONBAR_X_POS (10u)
#define HUD_ACTIONBAR_Y_POS (10u)

void draw_hud(RenderStaticInfo* render, Engine* engine) {
    WorldComponent *world = search_first_component(engine, COMP_WORLD);
    LevelInfoComponent* level_info = search_first_component(engine, COMP_LEVEL_INFO);

    assert(world != NULL);
    assert(level_info != NULL);

    uint32_t w = render->renderer->renderer_isometric.graphics.width;
    uint32_t h = render->renderer->renderer_isometric.graphics.height;

    int level_nr = level_info->nr;
    int level_digit_1 = level_nr % 10;
    int level_digit_2 = (level_nr / 10) % 10;
    renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_number[level_digit_1],
                         w - (HUD_LEVEL_X_POS + (HUD_NUMBER_WIDTH * (1))),
                         HUD_LEVEL_Y_POS);
    if (level_digit_2) {
        renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_number[level_digit_2],
                             w - (HUD_LEVEL_X_POS + (HUD_NUMBER_WIDTH * (2))),
                             HUD_LEVEL_Y_POS);
    }

    int pow10[5] = {10000, 1000, 100, 10, 1};
    int fps = get_fps(render, engine);
    int saw_non_null = 0;
    int first_non_null_i = -1;
    for (int i = 0; i < 5; i++) {
        int c = (fps / pow10[i]) % 10;
        saw_non_null = saw_non_null || c != 0;
        if (saw_non_null) {
            if (first_non_null_i == -1) {
                first_non_null_i = i;
            }
            renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_number[c],
                                 HUD_FPS_X_POS + (HUD_NUMBER_WIDTH * (i-first_non_null_i)),
                                 HUD_FPS_Y_POS);
        }
    }

    if (world->game_state == GAME_OVER) {
        renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_gameover,
                             (w-HUD_GAMEOVER_W) / 2,
                             (h-HUD_GAMEOVER_H) / 2);
    }
    if (world->game_state == GAME_WIN) {
        renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_gamewin,
                             (w-HUD_GAMEWIN_W) / 2,
                             (h-HUD_GAMEWIN_H) / 2);
    }

    // Editor HUD
    EditorToolBarComponent *editor = search_first_component(engine, COMP_EDITOR_TOOLBAR);
    if (editor != NULL) {
        renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_actionbar,
                             HUD_ACTIONBAR_X_POS,
                             h - (HUD_ACTIONBAR_H + HUD_ACTIONBAR_Y_POS));

        renderer_3d_hud_draw(render->renderer, render->renderer->tile_id_hud_actionbar_highlight,
                             HUD_ACTIONBAR_X_POS + (HUD_ACTIONBAR_HL_W * editor->selected_tool),
                             h - (HUD_ACTIONBAR_H + HUD_ACTIONBAR_Y_POS));
    }
}

Direction9 get_dir9_from_tile_xy(float x, float y) {
    float xx = x - floorf(x);
    float yy = y - floorf(y);
    assert(xx >= 0.0f);
    assert(yy >= 0.0f);
    assert(xx <= 1.0f);
    assert(yy <= 1.0f);
    if (xx < (1.0f/3.0f)) {
        if (yy < (1.0f/3.0f)) {
            return SW9;
        } else if (yy < (2.0f/3.0f)) {
            return W9;
        } else {
            return NW9;
        }
    } else if (xx < (2.0f/3.0f)) {
        if (yy < (1.0f/3.0f)) {
            return S9;
        } else if (yy < (2.0f/3.0f)) {
            return C9;
        } else {
            return N9;
        }
    } else {
        if (yy < (1.0f/3.0f)) {
            return SE9;
        } else if (yy < (2.0f/3.0f)) {
            return E9;
        } else {
            return NE9;
        }
    }
    fatal("unreachable code");
}

ScreenPosInfo screen_pos_convert(Engine* engine, int screen_x, int screen_y) {
    RenderStaticInfo* render = engine->render_static_info;
    ScreenPosInfo res = {0};
    res.ground_rect_it = NO_ENTITY;
    res.screen_pos[0] = screen_x;
    res.screen_pos[1] = screen_y - render->renderer->renderer_isometric.diamond_height/2;
    pos_screen_to_isometric(&render->renderer->renderer_isometric, res.screen_pos, res.iso_pos2);

    float s = 1.0f / 6.0f;
    EntityIterator ground_rect_it;
    search_entity_2(engine, COMP_GROUND_RECT, COMP_LOCATION, &ground_rect_it);
    res.has_ground_rect = 0;
    res.ground_offset_z = -127;
    glmc_vec3_set(res.tile_dir9_3d_pos, 0.f, 0.f, 0.f);
    glmc_vec3_set(res.tile_3d_pos, 0.f, 0.f, 0.f);

    //fallback
    t_vec3 iso_fallback = { res.iso_pos2[0],res.iso_pos2[1],0.0f }; //TODO: use default or closest ground height
    pos_isometric_to_3d(&render->renderer->renderer_isometric, iso_fallback, res.tile_3d_pos); //too snap to ground position

    while(next_entity(&ground_rect_it)) {
        LocationComponent* loc = get_component(engine, ground_rect_it.entity_id, COMP_LOCATION);
        GroundRectComponent *rect = get_component(engine, ground_rect_it.entity_id, COMP_GROUND_RECT);
        assert(rect != NULL);
        t_vec3 pos_rect0 = {rect->pos0[0] + loc->pos[0], rect->pos0[1] + loc->pos[1], rect->pos0[2] + loc->pos[2]};
        t_vec3 pos_rect1 = {rect->pos1[0] + loc->pos[0], rect->pos1[1] + loc->pos[1], rect->pos1[2] + loc->pos[2]};
        t_vec3 pos_iso0;
        t_vec3 pos_iso1;
        pos_3d_to_isometric(&render->renderer->renderer_isometric, pos_rect0, pos_iso0);
        pos_3d_to_isometric(&render->renderer->renderer_isometric, pos_rect1, pos_iso1);

        t_vec2 flat_pos0;
        t_vec2 flat_pos1;
        pos_isometric_flatten(&render->renderer->renderer_isometric, pos_iso0, flat_pos0);
        pos_isometric_flatten(&render->renderer->renderer_isometric, pos_iso1, flat_pos1);

//        printf("ground rect %d at %f,%f,%f -> %f,%f,%f -> %f,%f\n",
//               ground_rect_it.entity_id,
//               rect->pos[0], rect->pos[1], rect->pos[2],
//               pos_iso[0], pos_iso[1], pos_iso[2],
//               flat_pos[0], flat_pos[1]
//        );

        if (res.iso_pos2[0] >= flat_pos0[0] && res.iso_pos2[0] <= flat_pos1[0] &&
            res.iso_pos2[1] >= flat_pos0[1] && res.iso_pos2[1] <= flat_pos1[1]) {
//            printf("  %d,%d -> %f,%f matches ent=%d at %f,%f,%f:  x=[%f,%f] y=[%f,%f]\n",
//                   screen_x, screen_y,
//                   res.iso_pos2[0], res.iso_pos2[1],
//                   ground_rect_it.entity_id,
//                   rect->pos[0], rect->pos[1], rect->pos[2],
//                   flat_pos[0]-s, flat_pos[0]+s,
//                   flat_pos[1]-s, flat_pos[1]+s);
            res.has_ground_rect = 1;
            glmc_assign_vec3(res.tile_dir9_3d_pos, pos_rect0);
            glmc_assign_vec3(res.tile_3d_pos, pos_rect0);
            res.tile_3d_pos[0] = floorf(res.tile_3d_pos[0]);  //TODO: not correct   would have to come from GroundComponent
            res.tile_3d_pos[1] = floorf(res.tile_3d_pos[1]);  //TODO: not correct

            res.tile_dir9 = SW9; //TODO
            res.ground_rect_it = ground_rect_it.entity_id;

            res.ground_offset_z = 0;//rect->pos[2]; //TODO would have to come from GroundComponent
            return res;
        }
    }
    return res;
}

void system_render_update(Engine* engine) {
    RenderStaticInfo* render = engine->render_static_info;
    assert(engine != NULL);
    assert(render != NULL);
    assert(render->renderer != NULL);

    set_camera(render, engine);

    renderer_3d_begin_draw(render->renderer);

    EntityIterator draw_it;
    search_entity(engine, 0 /*no filter: iterate all entities*/, &draw_it);
    while (next_entity(&draw_it)) {
        EntityId drawable_entity_id = draw_it.entity_id;

        CarArtComponent* car_art = get_component(engine, drawable_entity_id, COMP_ART_CAR);
        GroundArtComponent* ground_art = get_component(engine, drawable_entity_id, COMP_ART_GROUND);
        BuildingArtComponent* building_art = get_component(engine, drawable_entity_id, COMP_ART_BUILDING);
        ColoredArtComponent* colored_art = get_component(engine, drawable_entity_id, COMP_ART_COLORED);
        VariousArtComponent* various_art = get_component(engine, drawable_entity_id, COMP_ART_VARIOUS);
        DirectionComponent* dir = get_component(engine, drawable_entity_id, COMP_DIRECTION);

        if (car_art == NULL && ground_art == NULL && building_art == NULL && colored_art == NULL && various_art == NULL) {
            continue;
        }

        LocationComponent *loc = get_component(engine, drawable_entity_id, COMP_LOCATION);
        if (loc == NULL) {
            continue;
        }

        t_vec3 entity_pos = {0};
        glmc_assign_vec3(entity_pos, loc->pos);

        if (ground_art != NULL) {
            t_vec3 ground_3d_pos = { entity_pos[0], entity_pos[1], entity_pos[2] + (float)ground_art->offset_z };
            renderer_3d_ground_draw(render->renderer, ground_art->art_id, ground_3d_pos);
        }

        if (building_art != NULL) {
            assert(dir != NULL);
            t_vec3 obj_3d_pos = {entity_pos[0], entity_pos[1], entity_pos[2] + (float)building_art->offset_z };
            renderer_3d_building_draw(render->renderer, building_art->art_id, direction8_to_direction(dir->direction), obj_3d_pos);
        }

        if (various_art != NULL) {
            assert(dir != NULL);
            Direction dir4 = direction8_to_direction(dir->direction);
            assert(dir4 >= 0);
            assert(dir4 < DIRECTION_COUNT);
            renderer_3d_various_draw(render->renderer, various_art->art_id, dir4, entity_pos, 0);
        }

        if (colored_art != NULL) {
            renderer_3d_colored_draw(render->renderer, colored_art->art_id, colored_art->color, entity_pos);
        }

        if (car_art != NULL) {
            assert(dir != NULL);
            UpDownComponent *updown = get_component(engine, drawable_entity_id, COMP_UPDOWN);
            renderer_3d_car_draw(render->renderer, car_art->art_id, entity_pos, dir->direction, updown->up_down);
        }
    }

    int mouse_screen_pos_x, mouse_screen_pos_y;
    /*uint32_t keymask = */SDL_GetMouseState(&mouse_screen_pos_x, &mouse_screen_pos_y);
    ScreenPosInfo screen_pos_info = screen_pos_convert(engine, mouse_screen_pos_x, mouse_screen_pos_y);

    //show GroundRectComponent if mouse is over it
    if (screen_pos_info.tile_3d_pos[0]*3 > BUFFER_MIN_X && screen_pos_info.tile_3d_pos[1]*3 > BUFFER_MIN_Y &&
        screen_pos_info.tile_3d_pos[0]*3 < BUFFER_MAX_X && screen_pos_info.tile_3d_pos[1]*3 < BUFFER_MAX_Y) {

        if (screen_pos_info.has_ground_rect) {
            if (has_component(engine, screen_pos_info.ground_rect_it, COMP_TILE_EDIT_INFO)) {
                //add whole tile highlight
                renderer_3d_various_draw(render->renderer, engine->render_static_info->renderer->art_various_id_tile_highlight, N, screen_pos_info.tile_3d_pos, 1);
            }

            //add sub tile highlight at height of GroundRectComponent
            ArtVariousId various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight;
            Direction dir = N;
            switch (screen_pos_info.tile_dir9) {
                case C9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight_center; dir = N; break; }
                case N9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight; dir = N; break; }
                case E9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight; dir = E; break; }
                case S9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight; dir = S; break; }
                case W9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight; dir = W; break; }
                case NE9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight_45; dir = N; break; }
                case SE9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight_45; dir = E; break; }
                case SW9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight_45; dir = S; break; }
                case NW9: { various_id = engine->render_static_info->renderer->art_various_id_tile_sub_highlight_45; dir = W; break; }
                default: fatal("Bad dir %d", screen_pos_info.tile_dir9);
            }

            renderer_3d_various_draw(render->renderer, various_id, dir, screen_pos_info.tile_dir9_3d_pos, 1);
        }
    }

    WorldComponent* world = search_first_component(engine, COMP_WORLD);
    if (world->show_debug) {
        //show COMP_ROAD
        EntityIterator road_it;
        search_entity_1(engine, COMP_ROAD, &road_it);
        while (next_entity(&road_it)) {
            EntityId road_entity_id = road_it.entity_id;
            LocationComponent *loc = get_component(engine, road_entity_id, COMP_LOCATION);
            assert(loc != NULL);
            t_vec3 rect0 = {loc->pos[0] - 0.15f, loc->pos[1] - 0.15f, loc->pos[2] };
            t_vec3 rect1 = {loc->pos[0] + 0.15f, loc->pos[1] + 0.15f, loc->pos[2] };
            renderer_3d_rect_draw(engine->render_static_info->renderer, rect0, rect1, 255, 255, 0, 255);
        }
    }

    draw_hud(render, engine);

    renderer_3d_end_draw(render->renderer);
}
