#include "es_memory_manager.h"
#include "../carbrain/carbrain_es_integration.h"

#include <assert.h>

void es_memory_manager_init(ESMemory* mem) {
    mem->next_entity_id = 0;

    for (ComponentId component_id = (ComponentId) 0; component_id < COMPONENT_ID_SIZE; component_id++) {
        for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
            memset(&mem->components[component_id][entity_id], 0, sizeof(AllComponent));
            mem->components[component_id][entity_id].free = 1;
            //TODO consider randomizing mem->components[component_id][entity_id].any_component
            //     because like malloc and stack variables, we don't want to make any promises about the content of components after create_component
        }
    }

    for (int x = GRIDLOC_CACHE_MIN_X; x <= GRIDLOC_CACHE_MAX_X; x++) {
        for (int y = GRIDLOC_CACHE_MIN_Y; y <= GRIDLOC_CACHE_MAX_Y; y++) {
            for (int i = 0; i < GRIDLOC_CACHE_MAX_PER_CELL; i++) {
                mem->gridloc_cache[x][y][i] = NO_ENTITY;
                mem->gridloc_cache_count[x][y] = GRIDLOC_CACHE_TOO_MANY;
            }
        }
    }
}

void add_to_cache(ESMemory* mem, int x, int y, EntityId entity_id) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            //add to 9 cache spots
            int cx = x+i;
            int cy = y+j;
            int x_index = cx - GRIDLOC_CACHE_MIN_X;
            int y_index = cy - GRIDLOC_CACHE_MIN_Y;
            if (cx >= GRIDLOC_CACHE_MIN_X && cx <= GRIDLOC_CACHE_MAX_X &&
                cy >= GRIDLOC_CACHE_MIN_Y && cy <= GRIDLOC_CACHE_MAX_Y) {
                int index = mem->gridloc_cache_count[x_index][y_index];
                if (index == GRIDLOC_CACHE_TOO_MANY || index == GRIDLOC_CACHE_MAX_PER_CELL - 1) {
                    //too much to store in this cache cell: fall back to searching all entities
                    mem->gridloc_cache_count[x_index][y_index] = GRIDLOC_CACHE_TOO_MANY;
                } else {
                    mem->gridloc_cache[x_index][y_index][index] = entity_id;
                    mem->gridloc_cache_count[x_index][y_index]++;
                }
            }
        }
    }
}

void update_gridloc_cache(ESMemory* mem) {
    for (int x = GRIDLOC_CACHE_MIN_X; x <= GRIDLOC_CACHE_MAX_X; x++) {
        for (int y = GRIDLOC_CACHE_MIN_Y; y <= GRIDLOC_CACHE_MAX_Y; y++) {
            for (int i = 0; i < GRIDLOC_CACHE_MAX_PER_CELL; i++) {
                int x_index = CACHE_X_TRANSFORM(x) - GRIDLOC_CACHE_MIN_X;
                int y_index = CACHE_Y_TRANSFORM(y) - GRIDLOC_CACHE_MIN_Y;
                assert(x_index >= 0);
                assert(y_index >= 0);
                assert(x_index < GRIDLOC_CACHE_DIM_X);
                assert(y_index < GRIDLOC_CACHE_DIM_Y);
                assert(i < GRIDLOC_CACHE_MAX_PER_CELL);
                mem->gridloc_cache[x_index][y_index][i] = NO_ENTITY;
                mem->gridloc_cache_count[x_index][y_index] = 0;
            }
        }
    }

    for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
        if (mem->components[COMP_LOCATION][entity_id].free)
            continue;

        LocationComponent* loc_comp = (LocationComponent*) &mem->components[COMP_LOCATION][entity_id].any_component;
        int x = CACHE_X_TRANSFORM(loc_comp->pos[0]);
        int y = CACHE_Y_TRANSFORM(loc_comp->pos[1]);
        add_to_cache(mem, x, y, entity_id);

//        MoveComponent *move_comp = (MoveComponent*) &mem->components[COMP_MOVE][entity_id].any_component;
//        if (!mem->components[COMP_MOVE][entity_id].free) {
//            int mx = CACHE_X_TRANSFORM(move_comp->from[0]);
//            int my = CACHE_Y_TRANSFORM(move_comp->from[1]);
//            if (mx != x || my != y) {
//                add_to_cache(mem, mx, my, entity_id);
//            }
//        }
    }

//    //safe fallback that disables cache:
//    //you can use this to rule out any problems with the cache. This will off course lower performance.
//    for (int x = GRIDLOC_CACHE_MIN_X; x <= GRIDLOC_CACHE_MAX_X; x++) {
//        for (int y = GRIDLOC_CACHE_MIN_Y; y <= GRIDLOC_CACHE_MAX_Y; y++) {
//            for (int i = 0; i < GRIDLOC_CACHE_MAX_PER_CELL; i++) {
//                mem->gridloc_cache[x_index][y_index][i] = NO_ENTITY;
//                mem->gridloc_cache_count[x_index][y_index] = GRIDLOC_CACHE_TOO_MANY;
//            }
//        }
//    }

//    //debug: print cache
//    printf("\n\n");
//    for (int x = GRIDLOC_CACHE_MIN_X; x <= GRIDLOC_CACHE_MAX_X; x++) {
//        for (int y = GRIDLOC_CACHE_MIN_Y; y <= GRIDLOC_CACHE_MAX_Y; y++) {
//            for (int i = 0; i < GRIDLOC_CACHE_MAX_PER_CELL; i++) {
//                printf("Cache at %d,%d has %d items\n", x, y, mem->gridloc_cache_count[x_index][y_index]);
//            }
//        }
//    }
//    printf("\n\n");
}

void es_memory_manager_free(ESMemory* mem) {
    //free any component that needs freeing. (most don't)

    for (EntityId entity_id = 0; entity_id < MAX_ENTITIES; entity_id++) {
        if (mem->components[COMP_WORLD][entity_id].free)
            continue;
    }
}

int has_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    fatal_if(entity_id == NO_ENTITY, "has_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(component_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(entity_id >= 0);
    return !engine->es_memory.components[component_id][entity_id].free;
}

void* get_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    fatal_if(entity_id == NO_ENTITY, "get_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(component_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(entity_id >= 0);
    if (engine->es_memory.components[component_id][entity_id].free)
        return NULL;
    assert(!engine->es_memory.components[component_id][entity_id].free);
    return &engine->es_memory.components[component_id][entity_id].any_component;
}

void* create_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    fatal_if(entity_id == NO_ENTITY, "create_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(entity_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(component_id >= 0);
    assert(component_id < COMPONENT_ID_SIZE);
    assert(engine->es_memory.components[component_id][entity_id].free);
    engine->es_memory.components[component_id][entity_id].free = 0;

    return &engine->es_memory.components[component_id][entity_id].any_component;
}

void free_component(Engine* engine, EntityId entity_id, ComponentId component_id) {
    fatal_if(entity_id == NO_ENTITY, "free_component(engine, entity_id==NO_ENTITY, component_id=%d)", component_id);
    assert(entity_id >= 0);
    assert(entity_id < MAX_ENTITIES);
    assert(component_id >= 0);
    assert(component_id < COMPONENT_ID_SIZE);

    assert(!engine->es_memory.components[component_id][entity_id].free);
    engine->es_memory.components[component_id][entity_id].free = 1;

    //if any components need cleanup, do it here:
    switch (component_id) {
        case COMP_CARBRAIN: {
            CarBrainComponent* carbrain = &engine->es_memory.components[component_id][entity_id].any_component.carbrain_component;
            free_CarBrainComponent(carbrain);
            break;
        }
        //TODO add any needed extra component cleanup
        default: {
            //nothing to do for any others
            break;
        }
    }
}

void* create_component_if_not_present(Engine* engine, EntityId entity_id, ComponentId component_id) {
    if (!has_component(engine, entity_id, component_id)) {
        return create_component(engine, entity_id, component_id);
    } else {
        return get_component(engine, entity_id, component_id);
    }
}

void free_component_if_present(Engine* engine, EntityId entity_id, ComponentId component_id) {
    if (has_component(engine, entity_id, component_id)) {
        free_component(engine, entity_id, component_id);
    }
}

int is_entity_free(Engine* engine, EntityId entity_id) {
    for (ComponentId component_id = (ComponentId) 0; component_id < COMPONENT_ID_SIZE; component_id++) {
        if (!engine->es_memory.components[component_id][entity_id].free) {
            return 0;
        }
    }
    return 1;
}

int entity_used_count(Engine* engine) {
    int res = 0;
    for (int entity_id =0; entity_id < MAX_ENTITIES; entity_id++) {
        if (!is_entity_free(engine, entity_id)) {
            res++;
        }
    }
    return res;
}

static_assert(MAX_ENTITIES < (~((EntityId)0u)) >> 1u, "MAX_ENTITIES is too large");
EntityId get_new_entity_id(Engine* engine) {
    //Performance warning: this naive method becomes very inefficient
    // if there are lot's of entities, and things start to get fragmented!
    EntityId start = engine->es_memory.next_entity_id % MAX_ENTITIES;
    for (EntityId i = 0; i < MAX_ENTITIES; i++) {
        EntityId res = (start + i) % MAX_ENTITIES; //see static_assert
        if (is_entity_free(engine, res)) {
            engine->es_memory.next_entity_id = res + 1;
            return res;
        }
    }
    fatal("Fatal error: Maximum number of entities used: %u", MAX_ENTITIES);
    return NO_ENTITY;
}

EntityId get_new_entity_with_id(Engine* engine, EntityId wanted_entity_id) {
    //this is used if you want a specific entity ID. That really only makes sense if you're loading a game.
    // you could just start using the entity off course (it's just a number!),
    // but this also checks if you didn't make mistake and are trying to use an in-use entity.
    if (!is_entity_free(engine, wanted_entity_id)) {
        fatal("Fatal error: Requested entity is not free: %u", wanted_entity_id);
        return NO_ENTITY;
    }
    return wanted_entity_id;
}

void delete_entity(Engine* engine, EntityId entity_id) {
    for (ComponentId component_id = (ComponentId) 0; component_id < COMPONENT_ID_SIZE; component_id++) {
        if (!engine->es_memory.components[component_id][entity_id].free) {
            free_component(engine, entity_id, component_id);
        }
    }
}
