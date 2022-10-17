#include "engine.h"
#include "system/move_system.h"
#include "system/camera_system.h"
#include "system/endlevel_system.h"
#include "system/levelloader_system.h"
#include "system/filter_system.h"
#include "system/edit_system.h"
#include "system/automove_c_system.h"
#include "system/dragging_system.h"

#include "../carbrain/carbrain_es_integration.h"
#include "system/carbrain_sensor_system.h"
#include "system/carbrain_decision_system.h"

#include <stdlib.h>
#include <assert.h>

#include <SDL_timer.h>

void engine_init(Engine* engine, Renderer3D* renderer) {
    es_memory_manager_init(&engine->es_memory);

    engine->render_static_info = render_static_info_alloc(renderer);
    //SDL_GetTicks() -> "Get  the  number of milliseconds since the SDL library initialization."
    engine->previous_tick = SDL_GetTicks();

    //levels are not yet initialised
    engine->levels = NULL;
    engine->level_count = 0;

    assert(engine->render_static_info != NULL);
}

void engine_free(Engine* e) {
    render_static_info_free(e->render_static_info);
    free(e->render_static_info);

    for (int i = 0; i < e->level_count; i++) {
        level_free(&e->levels[i]);
    }
    if (e->levels != NULL) {
        free(e->levels);
        e->levels = NULL;
    }

    EntityIterator all_it;
    search_entity(e, 0 /*no filter: iterate all entities*/, &all_it);
    while(next_entity(&all_it)) {
        EntityId entity_id = all_it.entity_id;
        delete_entity(e, entity_id);
    }
    es_memory_manager_free(&e->es_memory);

    //note: don't free graphics in engine
}

void engine_update(Engine* engine) {
    WorldComponent* world = search_first_component(engine, COMP_WORLD);
    int playing = world->game_state == GAME_PLAYING;
    int editing = world->game_state == GAME_EDIT;

    uint64_t curTicks = SDL_GetTicks();
    if (playing) {
        TimeComponent *time = search_first_component(engine, COMP_TIME);
        if (time != NULL) {
            time->time += curTicks - engine->previous_tick;
        }
    }
    engine->previous_tick = curTicks;
    system_levelloader_update(engine);
    system_edit_update(engine);
    if (playing) {
        system_carbrain_sensor_update(engine);
        if (is_carbrain_available()) {
            system_carbrain_decision_update(engine);
        } else {
            system_automove_c_update(engine);
        }
        system_move_update(engine);
        system_dragging_update(engine);
    }
    system_input_update(engine);
    if (playing || editing) {
        system_camera_update(engine);
    }
    if (playing) {
        system_endlevel_update(engine);
        system_filter_update(engine);
    }
    system_render_update(engine);
}
