#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include "assemblage.h"
#include "../carbrain/carbrain_es_integration.h"

void level_pos_to_entity_pos(int x, int y, int z, t_vec3 res, int middle) {
    float mid = middle ? 1.0f/6.0f : 0.0f;
    glmc_vec3_set(res, ((float)x/3.0f)+mid, ((float)y/3.0f)+mid, (float)z);
}

EntityId create_world(Engine *engine) {
    EntityId entity_id = get_new_entity_id(engine);

    WorldComponent* world = create_component(engine, entity_id, COMP_WORLD);
    world->show_debug = false;
    world->game_request = GAME_REQUEST_INIT;
    world->game_state = GAME_PLAYING;
    world->initial_level_nr = 0;
    assert(world->initial_level_nr >= 0);
    assert(world->initial_level_nr < engine->level_count);

    CameraComponent* cam_mem = create_component(engine, entity_id, COMP_CAMERA);
    glmc_vec3_set(cam_mem->current, 5.0f, 5.0f, 0.0f);
    glmc_assign_vec3(cam_mem->target, cam_mem->current);

    LevelInfoComponent* level_info = create_component(engine, entity_id, COMP_LEVEL_INFO);
    level_info->nr = 0;
    level_info->name = "none";
    assert(level_info->nr >= 0);
    assert(level_info->nr < engine->level_count);

    //TODO create AutoMoveControlComponent

    return entity_id;
}

EntityId create_time(Engine *engine) {
    EntityId entity_id = get_new_entity_id(engine);

    TimeComponent* time = create_component(engine, entity_id, COMP_TIME);
    time->time = 0;

    return entity_id;
}

EntityId create_ground_art(Engine *engine, t_vec3 pos, ArtGroundId art_id, int8_t offset_z) {
    EntityId art_entity_id = get_new_entity_id(engine);
//    printf("create_ground_art id=%u art_id=%u  @ %f,%f,%f\n", art_entity_id, art_id, pos[0], pos[1], pos[2]);

    GroundArtComponent* ground = create_component(engine, art_entity_id, COMP_ART_GROUND);
    ground->art_id = art_id;
    ground->offset_z = offset_z;

    LocationComponent* loc = create_component(engine, art_entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    return art_entity_id;
}

EntityId create_surface(Engine *engine, Level* level, int x, int y) {
    EntityId entity_id = get_new_entity_id(engine);

    int h = level->points[x][y].ground_height;

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    level_pos_to_entity_pos(x, y, h, loc->pos, 1);

    if (level->points[x][y].ground_material == ART_ROAD) {
        create_component(engine, entity_id, COMP_ROAD);
    }

    return entity_id;
}


EntityId create_building_ground_floor(Engine* engine, t_vec3 pos, ArtBuildingId art_id, Direction facing_dir, int8_t offset_z) {
    EntityId entity_id = get_new_entity_id(engine);

    BuildingArtComponent* obj = create_component(engine, entity_id, COMP_ART_BUILDING);
    obj->art_id = art_id;
    obj->offset_z = offset_z;

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    DirectionComponent* directioncomponent = create_component(engine, entity_id, COMP_DIRECTION);
    directioncomponent->direction = direction_to_direction8(facing_dir);

    return entity_id;
}


EntityId create_building_level(Engine* engine, t_vec3 pos, ArtBuildingId art_id, Direction facing_dir, int8_t offset_z) {
    EntityId entity_id = get_new_entity_id(engine);

    BuildingArtComponent* obj = create_component(engine, entity_id, COMP_ART_BUILDING);
    obj->art_id = art_id;
    obj->offset_z = offset_z;

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    DirectionComponent* directioncomponent = create_component(engine, entity_id, COMP_DIRECTION);
    directioncomponent->direction = direction_to_direction8(facing_dir);

    return entity_id;
}


EntityId create_building_roof(Engine* engine, t_vec3 pos, ArtBuildingId art_id, Direction facing_dir, int8_t offset_z) {
    EntityId entity_id = get_new_entity_id(engine);

    BuildingArtComponent* obj = create_component(engine, entity_id, COMP_ART_BUILDING);
    obj->art_id = art_id;
    obj->offset_z = offset_z;

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    DirectionComponent* directioncomponent = create_component(engine, entity_id, COMP_DIRECTION);
    directioncomponent->direction = direction_to_direction8(facing_dir);

    return entity_id;
}

ArtColor color_translate(LevelItemColor color) {
    if (color == LEVEL_COLOR_GREY) { return ART_COLOR_GREY; }
    if (color == LEVEL_COLOR_RAINBOW) { return ART_COLOR_RAINBOW; }
    if (color == LEVEL_COLOR_0) { return ART_COLOR_RED; }
    if (color == LEVEL_COLOR_1) { return ART_COLOR_GREEN; }
    if (color == LEVEL_COLOR_2) { return ART_COLOR_BLUE; }
    if (color == LEVEL_COLOR_3) { return ART_COLOR_MAGENTA; }
    if (color == LEVEL_COLOR_4) { return ART_COLOR_CYAN; }
    fatal("Unsupported COLOR \"%d\"", color); return 0;
}

EntityId create_decoration(Engine *engine, float *pos, int art_variant, bool art_only) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    DirectionComponent* directioncomponent = create_component(engine, entity_id, COMP_DIRECTION);
//    Direction d = rand() % DIRECTION_COUNT;
    Direction d = N;
    assert(d >= 0);
    assert(d < DIRECTION_COUNT);
    directioncomponent->direction = direction_to_direction8(d);
    assert(directioncomponent->direction >= 0);
    assert(directioncomponent->direction < DIRECTION8_COUNT);
    assert(direction8_to_direction(directioncomponent->direction) >= 0);
    assert(direction8_to_direction(directioncomponent->direction) < DIRECTION_COUNT);

    VariousArtComponent* art = create_component(engine, entity_id, COMP_ART_VARIOUS);
    assert(art_variant >= 0);
    assert(art_variant < 5);
    if (art_variant == 4) {
        art->art_id = renderer_3d_various_lookup(engine->render_static_info->renderer, ART_ROCK, 0);
    } else {
        art->art_id = renderer_3d_various_lookup(engine->render_static_info->renderer, ART_PLANTS, art_variant);
    }

//    if (!art_only) {
        //decorations are always art only
//    }

    return entity_id;
}

EntityId create_crate(Engine *engine, t_vec3 pos, LevelItemColor color, bool art_only) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    ColoredArtComponent* art = create_component(engine, entity_id, COMP_ART_COLORED);
    art->art_id = renderer_3d_colored_lookup(engine->render_static_info->renderer, ART_CRATE, 0);
    art->color = color_translate(color);

    if (!art_only) {
        //TODO
        DraggableComponent* crate = create_component(engine,entity_id,COMP_DRAGGABLE);
        crate->dragged_by=NO_ENTITY;
        crate->next_crate=NO_ENTITY;
    }

    return entity_id;
}

EntityId create_marker(Engine *engine, t_vec3 pos, LevelItemColor color, bool art_only) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    ColoredArtComponent* art = create_component(engine, entity_id, COMP_ART_COLORED);
    art->art_id = renderer_3d_colored_lookup(engine->render_static_info->renderer, ART_BUTTON_SQUARE, 0);
    assert(art->art_id != ART_COLORED_ID_NONE);
    art->color = color_translate(color);

    if (!art_only) {
        //TODO
    }

    return entity_id;
}

EntityId create_filter(Engine *engine, float *pos, int count, bool art_only) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    VariousArtComponent* art = create_component(engine, entity_id, COMP_ART_VARIOUS);
    art->art_id = renderer_3d_various_lookup(engine->render_static_info->renderer, ART_FILTER, count);
    assert(art->art_id != ART_VARIOUS_ID_NONE);

    DirectionComponent* direction_comp = create_component(engine, entity_id, COMP_DIRECTION);
    direction_comp->direction = N;

    if (!art_only) {
        FilterComponent* filter= create_component(engine,entity_id,COMP_FILTER);
        filter->required_crates=count;
        //TODO
    }

    return entity_id;
}

EntityId create_car(Engine *engine, t_vec3 pos, LevelItemColor color, Direction dir, bool art_only, int level_nr, int car_index) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent *loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    CarArtComponent* art = create_component(engine, entity_id, COMP_ART_CAR);
    //There are 7 colors and 11 cars at the moment. So this is not strictly needed.
    art->art_id = color % engine->render_static_info->renderer->art_car_count;

    DirectionComponent* directioncomponent = create_component(engine, entity_id, COMP_DIRECTION);
    directioncomponent->direction = direction_to_direction8(dir);

    UpDownComponent* updown_component = create_component(engine, entity_id, COMP_UPDOWN);
    updown_component->up_down = 0;

    if (!art_only) {
        //TODO
        create_component(engine,entity_id,COMP_EXIT_ACTIVATOR);
        create_component(engine, entity_id, COMP_INPUTRECEIVER_OPTION);
        create_component(engine,entity_id,COMP_DRAGGER);


        //make car clickable
        GroundRectComponent* rect = create_component(engine, entity_id, COMP_GROUND_RECT);
        float half_1x1 = 1.0f / 6.0f;
        glmc_vec3_set(rect->pos0, -half_1x1, -half_1x1, 0.0f);
        glmc_vec3_set(rect->pos1, half_1x1, half_1x1, 0.0f);
    }

    return entity_id;
}

EntityId create_exit(Engine *engine, float *pos, Direction direction, bool art_only) {
    EntityId entity_id = get_new_entity_id(engine);

    LocationComponent *loc = create_component(engine, entity_id, COMP_LOCATION);
    glmc_assign_vec3(loc->pos, pos);

    DirectionComponent* dir = create_component(engine, entity_id, COMP_DIRECTION);
    dir->direction = direction_to_direction8(direction);

    VariousArtComponent* art = create_component(engine, entity_id, COMP_ART_VARIOUS);
    art->art_id = renderer_3d_various_lookup(engine->render_static_info->renderer, ART_FLAG, 0);

    if (!art_only) {
        create_component(engine, entity_id, COMP_EXIT);
        ExitedComponent* exit_animation= create_component(engine,entity_id,COMP_LEVEL_END_ANIMATION);
        exit_animation->sleep=350u;
        exit_animation->dir_tuned=0;
        //TODO
    }
    return entity_id;
}

typedef struct ArtLookupResult {
    ArtGroundId art_id;
    int8_t offset_z;
    uint8_t min_height;
} ArtLookupResult;

ArtLookupResult lookup_art(Renderer3D* renderer, ArtGround3D* art_3d_search) {
    ArtLookupResult res;
    res.art_id = ART_GROUND_ID_NONE;
    res.offset_z = 0;
    res.min_height = 0;

    res.min_height = art_3d_search->height[ART_POS_CENTER];
    for (int i = 0; i < ART_POS__COUNT; i++) {
        int cur = art_3d_search->height[i];
        res.min_height = res.min_height < cur ? res.min_height : cur;
    }
    for (int i = 0; i < ART_POS__COUNT; i++) {
        art_3d_search->height[i] -= res.min_height;
    }

    for (int i = 0; i <= 6; i+=2) {
        res.art_id = renderer_3d_ground_lookup(renderer, art_3d_search);
        if (res.art_id != ART_GROUND_ID_NONE) {
            break;
        }

        for (int j = 0; j < ART_POS__COUNT; j++) {
            art_3d_search->height[j] += 2;
        }
        res.offset_z -= 2;
    }

    return res;
}


void set_basic_level_entities(Engine* engine, Level *level) {
    LevelInfoComponent* level_info = search_first_component(engine, COMP_LEVEL_INFO);
    assert(level_info != NULL);
    level_info->nr = level->nr;
    level_info->name = level->name;

    SDL_SetWindowTitle(
            engine->render_static_info->renderer->renderer_isometric.graphics.sdl_window,
            level_info->name
    );
}

void create_tile_entities(Engine* engine, Level *level, bool art_only) {
    assert(level->width > 0);
    assert(level->height > 0);
    assert(level->width % 3 == 0);
    assert(level->height % 3 == 0);

    if (!art_only) {
        for (int x = 0; x < level->width; x++) {
            for (int y = 0; y < level->height; y++) {
                create_surface(engine, level, x, y);
            }
        }
    }

    int car_index = 0;
    for (size_t i = 0; i < level->item_count; i++) {
        LevelItem* item = &level->items[i];

        t_vec3 pos;
        level_pos_to_entity_pos(item->x, item->y,
                                level->points[item->x][item->y].ground_height, // + item->z_offset,
                                pos, 1);

        switch (item->type) {
            case CAR: {
                create_car(engine, pos, item->color, item->dir, art_only, level->nr, car_index++);
                break;
            }
            case EXIT: {
                create_exit(engine, pos, item->dir, art_only);
                break;
            }
            case CRATE: {
                create_crate(engine, pos, item->color, art_only);
                break;
            }
            case FILTER: {
                create_filter(engine, pos, item->count, art_only);
                break;
            }
            case DECORATION: {
                create_decoration(engine, pos, item->art_variant, art_only);
                break;
            }
            case MARKER: {
                create_marker(engine, pos, item->color, art_only);
                break;
            }
            case BUILDING: {
                //nothing to do here, handled as surface in entity system
                break;
            }
        }
    }

    update_gridloc_cache(&engine->es_memory);
}

void leveledit_entities_to_level(Engine* engine, Level *level) {
    LevelInfoComponent* level_info = search_first_component(engine, COMP_LEVEL_INFO);
    EditorInfoComponent* editor_info = search_first_component(engine, COMP_EDITOR_INFO);

    uint8_t w = editor_info->level_width;
    uint8_t h = editor_info->level_height;

    level_init(level, w, h, 1, level_info->nr, level_info->name, editor_info->level_base_filename);

    int item_count = 0;
    int no_item_count = 0;
    EntityIterator leveledit_it;
    search_entity_1(engine, COMP_TILE_EDIT_INFO, &leveledit_it);
    while (next_entity(&leveledit_it)) {
        EntityId edit_entity_id = leveledit_it.entity_id;
        TileEditInfoComponent *edit = get_component(engine, edit_entity_id, COMP_TILE_EDIT_INFO);
        assert(edit != NULL);
        level->points[edit->pos[0]][edit->pos[1]].ground_material = edit->ground_material;
        level->points[edit->pos[0]][edit->pos[1]].ground_height = edit->ground_material == ART_WATER ? 2 : 4;

        if (edit->hasItem) {
            item_count++;
        } else {
            no_item_count++;
        }
    }

    level->item_count = item_count;
    level->items = realloc(level->items, level->item_count * sizeof(LevelItem));
    memset(level->items, 0, level->item_count * sizeof(LevelItem)); //needed: realloc does not set to 0

    int i = 0;
    search_entity_1(engine, COMP_TILE_EDIT_INFO, &leveledit_it);
    while (next_entity(&leveledit_it)) {
        EntityId edit_entity_id = leveledit_it.entity_id;
        TileEditInfoComponent *edit = get_component(engine, edit_entity_id, COMP_TILE_EDIT_INFO);
        assert(edit != NULL);

        if (edit->hasItem) {
            level->items[i].x = edit->pos[0];
            level->items[i].y = edit->pos[1];
            level->items[i].type = edit->item.type;
            level->items[i].color = edit->item.color;
            level->items[i].dir = edit->item.dir;
            level->items[i].count = edit->item.count;
            level->items[i].art_variant = edit->item.art_variant;
            i++;
        }
    }
}

void create_edit_level_entities(Engine* engine, Level *level) {
    assert(level->width > 0);
    assert(level->height > 0);
    assert(level->width % 3 == 0);
    assert(level->height % 3 == 0);

    EntityId** edits = calloc(level->width, sizeof(EntityId*));
    for (int x = 0; x < level->width; x++) {
        edits[x] = calloc(level->height, sizeof(EntityId));
        for (int y = 0; y < level->height; y++) {
            edits[x][y] = NO_ENTITY;
        }
    }

    for (int x = 0; x < level->width; x++) {
        for (int y = 0; y < level->height; y++) {
            EntityId entity_id = get_new_entity_id(engine);
            edits[x][y] = entity_id;

            int h = level->points[x][y].ground_height;

            LocationComponent* loc = create_component(engine, entity_id, COMP_LOCATION);
            level_pos_to_entity_pos(x, y, h, loc->pos, 1);

            GroundRectComponent* rect = create_component(engine, entity_id, COMP_GROUND_RECT);
            glmc_vec3_set(rect->pos0, -1.0f/6.0f, -1.0f/6.0f, 0.0f);
            glmc_vec3_set(rect->pos1, 1.0f/6.0f, 1.0f/6.0f, 0.0f);

            TileEditInfoComponent *leveledit = create_component(engine, entity_id, COMP_TILE_EDIT_INFO);
            leveledit->pos[0] = x;
            leveledit->pos[1] = y;
            leveledit->ground_material = level->points[x][y].ground_material;
        }
    }

    for (size_t i = 0; i < level->item_count; i++) {
        LevelItem* item = &level->items[i];

        EntityId entity_id = edits[item->x][item->y];
        assert(entity_id != NO_ENTITY);

        TileEditInfoComponent *tile_edit = get_component(engine, entity_id, COMP_TILE_EDIT_INFO);
        assert(tile_edit != NULL);
        assert(!tile_edit->hasItem);
        tile_edit->hasItem = true;
        tile_edit->item = *item;
        assert(tile_edit->hasItem);
    }

    for(int x = 0; x < level->width; x++) {
        free(edits[x]);
    }
    free(edits);

    update_gridloc_cache(&engine->es_memory);
}

void clear_level(Engine* engine, bool clear_art_only) {
    if (clear_art_only) {
        ComponentId art_to_delete[] = {
                COMP_ART_GROUND, COMP_ART_BUILDING, COMP_ART_COLORED,
                COMP_ART_VARIOUS, COMP_ART_CAR };

        for (int i = 0; i < sizeof(art_to_delete) / sizeof(ComponentId); i++) {
            EntityIterator art_it;
            search_entity_1(engine, art_to_delete[i], &art_it);
            while (next_entity(&art_it)) {
                EntityId art_entity_id = art_it.entity_id;
                delete_entity(engine, art_entity_id);
            }
        }
    } else {
        EntityIterator it;
        search_entity_1(engine, COMP_LOCATION, &it);
        while(next_entity(&it)) {
            EntityId entity_id = it.entity_id;
            delete_entity(engine, entity_id);
        }
    }
    update_gridloc_cache(&engine->es_memory);
}

void create_surface_art_level_entities(Engine* engine, Level *level) {
    uint8_t w = level->width;
    uint8_t h = level->height;

    LevelItem*** items = calloc(level->width, sizeof(LevelItem*));
    for (int x = 0; x < level->width; x++) {
        items[x] = calloc(level->height, sizeof(LevelItem *));
    }
    for (int i = 0; i < level->item_count; i++) {
        items[level->items[i].x][level->items[i].y] = &level->items[i];
    }

    for (int x = 0; x < w; x+=3) {
        for (int y = 0; y < h; y+=3) {
            //a little hack to get fixed random buildings for each level
            srand((x * 17) + (y * 13) + level->nr);

            //The 9 points for easy access
            LevelPoint* points_dir9[DIRECTION9_COUNT];
            int8_t ground_heights_dir9[DIRECTION9_COUNT];
            uint8_t min_ground_height = 0xFF;

            //Find the 9 points
            for (Direction9 d = 0; d < DIRECTION9_COUNT; d++) {
                int xd = RELATIVE9_X(d, x + 1);
                int yd = RELATIVE9_Y(d, y + 1);
                points_dir9[d] = &level->points[xd][yd];
                ground_heights_dir9[d] = (int8_t) points_dir9[d]->ground_height;
                min_ground_height = MIN(min_ground_height, ground_heights_dir9[d]);
            }

            LevelItem* center_item = items[x+1][y+1];
            uint8_t building_story_count = center_item != NULL && center_item->type == BUILDING ? center_item->count : 0;
            uint8_t building_facing = center_item != NULL ? center_item->dir : N;

            ArtGround3D art_3d_search;
            art_3d_search.materials[ART_POS_CENTER] = points_dir9[C9]->ground_material;
            art_3d_search.materials[ART_POS_N] = points_dir9[N9]->ground_material;
            art_3d_search.materials[ART_POS_NW] = points_dir9[NW9]->ground_material;
            art_3d_search.materials[ART_POS_W] = points_dir9[W9]->ground_material;
            art_3d_search.materials[ART_POS_SW] = points_dir9[SW9]->ground_material;
            art_3d_search.materials[ART_POS_S] = points_dir9[S9]->ground_material;
            art_3d_search.materials[ART_POS_SE] = points_dir9[SE9]->ground_material;
            art_3d_search.materials[ART_POS_E] = points_dir9[E9]->ground_material;
            art_3d_search.materials[ART_POS_NE] = points_dir9[NE9]->ground_material;
            art_3d_search.alt_art_index = 0;

            art_3d_search.height[ART_POS_CENTER] = ground_heights_dir9[C9];
            art_3d_search.height[ART_POS_N] = ground_heights_dir9[N9];
            art_3d_search.height[ART_POS_NW] = ground_heights_dir9[NW9];
            art_3d_search.height[ART_POS_W] = ground_heights_dir9[W9];
            art_3d_search.height[ART_POS_SW] = ground_heights_dir9[SW9];
            art_3d_search.height[ART_POS_S] = ground_heights_dir9[S9];
            art_3d_search.height[ART_POS_SE] = ground_heights_dir9[SE9];
            art_3d_search.height[ART_POS_E] = ground_heights_dir9[E9];
            art_3d_search.height[ART_POS_NE] = ground_heights_dir9[NE9];

            ArtLookupResult lookupArt = lookup_art(engine->render_static_info->renderer, &art_3d_search);

            ArtGroundId art_ground_id = lookupArt.art_id;

            fatal_if(min_ground_height != lookupArt.min_height,
                     "min_ground_height (%u) != lookupArt.min_height (%u) at %d,%d\n",
                     min_ground_height, lookupArt.min_height, x, y);

            if (building_story_count == 0) {
                if (art_ground_id != ART_GROUND_ID_NONE) {
//                if (art_ground_id != ART_GROUND_ID_NONE && 0) {
                    //ART for 3x3 found, use it
                    t_vec3 pos;
                    level_pos_to_entity_pos(x, y, min_ground_height, pos, 0);
                    create_ground_art(engine, pos, art_ground_id, lookupArt.offset_z);
                } else {
                    //no 3x3 art found. Use 9 1x1 arts
                    t_vec3 pos;
                    for (int xx = 0; xx < 3; xx++) {
                        for (int yy = 0; yy < 3; yy++) {
                            LevelPoint* point = &level->points[x+xx][y+yy];
                            level_pos_to_entity_pos(x+xx, y+yy, point->ground_height, pos, 0);
                            pos[2] += 0.7f;  //correction factor
                            ArtGroundId ground_id = engine->render_static_info->renderer->art_ground_id_base_1x1[point->ground_material];
//                            printf("Fallback ground ID for %ux%u -> %fx%f: %u\n", x+xx, y+yy, pos[0], pos[1], ground_id);
                            create_ground_art(engine, pos, ground_id, 2);
                        }
                    }
                }
            }

            if (building_story_count > 0) {
                //shared random colors (same for ground levels, all floors, and roof (red only for roof))
                BuildingColor wall_color = RAND_BUILDING_SHARED_COLOR;

                //shared random window type (same for ground levels, all floors)
                BuildingWindow window_type = RAND_BUILDING_WINDOW;

                //ground floor
                ArtBuildingId art_building_id = renderer_3d_building_lookup(
                        engine->render_static_info->renderer,
                        ART_BUILDING_GROUND_FLOOR,
                        -1,
                        wall_color,
                        window_type,
                        -1, -1,
                        0xFF
                );
                if (art_building_id == ART_BUILDING_ID_NONE) {
                    art_building_id = renderer_3d_building_lookup(
                            engine->render_static_info->renderer,
                            ART_BUILDING_GROUND_FLOOR,
                            -1,
                            wall_color,
                            -1, -1, -1,
                            0xFF
                    );
                }
                if (art_building_id == ART_BUILDING_ID_NONE) {
                    art_building_id = renderer_3d_building_lookup(
                            engine->render_static_info->renderer,
                            ART_BUILDING_GROUND_FLOOR,
                            -1, -1, -1, -1, -1,
                            0xFF
                    );
                }
                assert(art_building_id != ART_BUILDING_ID_NONE);

                t_vec3 pos;
                level_pos_to_entity_pos(x, y, min_ground_height, pos, 0);
                create_building_ground_floor(engine, pos, art_building_id, building_facing, -4);

                art_building_id = renderer_3d_building_lookup(
                        engine->render_static_info->renderer,
                        ART_BUILDING_STORY,
                        -1,
                        wall_color,
                        window_type,
                        -1, -1,
                        0xFF
                );
                if (art_building_id == ART_BUILDING_ID_NONE) {
                    art_building_id = renderer_3d_building_lookup(
                            engine->render_static_info->renderer,
                            ART_BUILDING_STORY,
                            -1,
                            wall_color,
                            -1, -1, -1,
                            0xFF
                    );
                }
                if (art_building_id == ART_BUILDING_ID_NONE) {
                    art_building_id = renderer_3d_building_lookup(
                            engine->render_static_info->renderer,
                            ART_BUILDING_STORY,
                            -1, -1, -1, -1, -1,
                            0xFF
                    );
                }
                assert(art_building_id != ART_BUILDING_ID_NONE);
                for (int i = 1; i < building_story_count; i++) {
                    //xth level
                    level_pos_to_entity_pos(x, y, min_ground_height+ (i * 4), pos, 0);
                    create_building_level(engine, pos, art_building_id, building_facing, 0);
                }

                //roof
                art_building_id = renderer_3d_building_lookup(
                        engine->render_static_info->renderer,
                        ART_BUILDING_ROOF,
                        -1,
                        wall_color,
                        -1, -1, -1,
                        0xFF
                );
                if (art_building_id == ART_BUILDING_ID_NONE) {
                    art_building_id = renderer_3d_building_lookup(
                            engine->render_static_info->renderer,
                            ART_BUILDING_ROOF,
                            -1,
                            -1, -1, -1, -1,
                            0xFF
                    );
                }
                assert(art_building_id != ART_BUILDING_ID_NONE);
                ArtBuilding3D *check_roof = &engine->render_static_info->renderer->art_building[art_building_id];
                assert(check_roof->type == ART_BUILDING_ROOF);
                level_pos_to_entity_pos(x, y, min_ground_height + (building_story_count * 4), pos, 0);
                create_building_roof(engine, pos, art_building_id, building_facing, 0);
            }
        }
    }

    update_gridloc_cache(&engine->es_memory);

    for (int x = 0; x < level->width; x++) {
        free(items[x]);
    }
    free(items);
}


void create_base_entities(Engine *engine) {
    //check if levels have already been loaded
    assert(engine->levels != NULL);
    assert(engine->level_count > 0);

    EntityId world = create_world(engine);
    EntityId time = create_time(engine);
}

void update_editor_info(Engine* engine, Level *level) {
    EditorInfoComponent* editor_info = search_first_component(engine, COMP_EDITOR_INFO);
    assert(editor_info != NULL);
    editor_info->level_height = level->height;
    editor_info->level_width = level->width;
    editor_info->level_base_filename = level->source_file_name;
}

void set_level_entities(Engine* engine, Level *level,
                        bool set_editor_info, bool create_editor_entities, bool create_game_entities) {
    set_basic_level_entities(engine, level);

    if (set_editor_info) {
        update_editor_info(engine, level);
    }

    if (create_editor_entities) {
        create_edit_level_entities(engine, level);
    }

    create_surface_art_level_entities(engine, level);
    create_tile_entities(engine, level, !create_game_entities);
}
