#include "es_memory_manager_collections.h"
#include "es_memory_manager.h"
#include "engine.h"

#include <stdlib.h>
#include <assert.h>

void search_component(Engine* engine,
                      ComponentId component_id,
                      ComponentIterator* res) {
    res->engine = engine;
    res->component_id = component_id;
    res->entity_id = (EntityId) -1; //bit of a hack
}

int next_component(ComponentIterator* res) {
    for (EntityId entity_id = res->entity_id+1; entity_id < MAX_ENTITIES; entity_id++) {
        if (!res->engine->es_memory.components[res->component_id][entity_id].free) {
            res->entity_id = entity_id;
            res->comp = &res->engine->es_memory.components[res->component_id][entity_id].any_component;
            return 1;
        }
    }
    return 0;
}

void* search_first_component(Engine* engine, ComponentId component_id) {
    ComponentIterator it;
    search_component(engine, component_id, &it);
    if (next_component(&it))
        return it.comp;
    else
        return NULL;
}



static int get_requires_component(component_id_filter_t component_id_filter, ComponentId component_id) {
    component_id_filter_t mask = ((component_id_filter_t)1) << component_id;
    return (mask & component_id_filter) != 0;
}
component_id_filter_t set_requires_component(component_id_filter_t component_id_filter, ComponentId component_id) {
    component_id_filter_t mask = ((component_id_filter_t)1) << component_id;
    return component_id_filter | mask;
}


void search_entity(Engine *engine, component_id_filter_t component_id_filter, EntityIterator *res) {
    res->engine = engine;
    res->component_id_filter = component_id_filter;
    res->entity_id = (EntityId) -1; //bit of a hack
    res ->search_mode = SEARCHMODE_ANY;
    res->pos[0] = 0;
    res->pos[1] = 0;
    res->pos[2] = 0;

    static_assert(COMPONENT_ID_SIZE <= sizeof(component_id_filter_t) * 8,
                  "component_id_filter_t should have enough bits to hold all components");
}

void search_entity_1(Engine *engine, ComponentId component_id1, EntityIterator *res) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    search_entity(engine, component_id_filter, res);
}
void search_entity_2(Engine *engine, ComponentId component_id1, ComponentId component_id2, EntityIterator *res) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    search_entity(engine, component_id_filter, res);
}
void search_entity_3(Engine *engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3,
                     EntityIterator *res) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    search_entity(engine, component_id_filter, res);
}

void
search_entity_by_location_1(Engine *engine, t_vec3 pos, float max_distance, int ignore_z, ComponentId component_id1,
                            EntityIterator *res) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, COMP_LOCATION);
    search_entity(engine, component_id_filter, res);
    res->search_mode = ignore_z ? SEARCHMODE_DIST_IGNORE_Z : SEARCHMODE_DIST;
    res->max_distance_squared = SQUARE(max_distance);
    res->pos_cache_index = 0;
    glmc_assign_vec3(res->pos, pos);
}

void
search_entity_by_location_2(Engine *engine, t_vec3 pos, float max_distance, int ignore_z, ComponentId component_id1,
                            ComponentId component_id2, EntityIterator *res) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, COMP_LOCATION);
    search_entity(engine, component_id_filter, res);
    res->search_mode = ignore_z ? SEARCHMODE_DIST_IGNORE_Z : SEARCHMODE_DIST;
    res->max_distance_squared = SQUARE(max_distance);
    res->pos_cache_index = 0;
    glmc_assign_vec3(res->pos, pos);
}

EntityId search_first_entity_with_mask(Engine *engine, component_id_filter_t component_id_filter) {
    EntityIterator it;
    search_entity(engine, component_id_filter, &it);
    if (next_entity(&it))
        return it.entity_id;
    else
        return NO_ENTITY;
}
EntityId search_first_entity_1(Engine *engine, ComponentId component_id1) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    return search_first_entity_with_mask(engine, component_id_filter);
}
EntityId search_first_entity_2(Engine *engine, ComponentId component_id1, ComponentId component_id2) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    return search_first_entity_with_mask(engine, component_id_filter);
}
EntityId search_first_entity_3(Engine *engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3) {
    component_id_filter_t component_id_filter = set_requires_component(0, component_id1);
    component_id_filter = set_requires_component(component_id_filter, component_id2);
    component_id_filter = set_requires_component(component_id_filter, component_id3);
    return search_first_entity_with_mask(engine, component_id_filter);
}


#define DEBUG_CACHE 1


int next_entity(EntityIterator* it) {
    int cache_miss_check = 0;

    //if searching based on gridpos, first try to use cache
    if ((it->search_mode == SEARCHMODE_DIST || it->search_mode == SEARCHMODE_DIST_IGNORE_Z) &&
            it->max_distance_squared < MAX_CACHE_DISTANCE_SQUARE) {
        if (it->pos[0] >= GRIDLOC_CACHE_MIN_X && it->pos[1] >= GRIDLOC_CACHE_MIN_Y &&
            it->pos[0] < GRIDLOC_CACHE_MAX_X && it->pos[1] < GRIDLOC_CACHE_MAX_Y) {
            int x_index = CACHE_X_TRANSFORM(it->pos[0]) - GRIDLOC_CACHE_MIN_X;
            int y_index = CACHE_X_TRANSFORM(it->pos[1]) - GRIDLOC_CACHE_MIN_Y;
            assert(x_index >= 0);
            assert(y_index >= 0);
            assert(x_index < GRIDLOC_CACHE_DIM_X);
            assert(y_index < GRIDLOC_CACHE_DIM_Y);
            int cache_count = it->engine->es_memory.gridloc_cache_count[x_index][y_index];
        if (cache_count == 0) {
            //cache says there's no entity at this location
            return 0;
        }
        if (cache_count != GRIDLOC_CACHE_TOO_MANY) {
            assert(cache_count > 0);
            assert(cache_count < GRIDLOC_CACHE_MAX_PER_CELL);
            while (1) {
                if (it->pos_cache_index == cache_count) {
                    //already returned all found entities
                    if (DEBUG_CACHE) {
                        cache_miss_check = 1;
                        break;
                    } else {
                        return 0;
                    }
                }

                assert(it->pos_cache_index < cache_count);
                assert(it->pos_cache_index < GRIDLOC_CACHE_MAX_PER_CELL);
                    EntityId entity_id = it->engine->es_memory.gridloc_cache[x_index][y_index][it->pos_cache_index++];
                assert(entity_id != GRIDLOC_CACHE_TOO_MANY);
                assert(entity_id != NO_ENTITY);

                int matches_all = 1;

                LocationComponent *loc = (LocationComponent *) &it->engine->es_memory.components[COMP_LOCATION][entity_id].any_component;

                float dist_square = SQUARE(loc->pos[0] - it->pos[0]) + SQUARE(loc->pos[1] - it->pos[1]);
                if (it->search_mode == SEARCHMODE_DIST) {
                    dist_square += SQUARE(loc->pos[2] - it->pos[2]);
                }
                if (dist_square > it->max_distance_squared) {
                    //if not close enough, try next in cache
                    matches_all = 0;
                    continue;
                }

                //we now know position the position matches. check if filters match as well.
                for (ComponentId component_id = 0; component_id < COMPONENT_ID_SIZE; component_id++) {
                    if (get_requires_component(it->component_id_filter, component_id) &&
                        it->engine->es_memory.components[component_id][entity_id].free) {
                        //no component match, try next in cache
                        matches_all = 0;
                        break;
                    }
                }

                if (matches_all) {
                    //match, return result
                    it->entity_id = entity_id;
                    return 1;
                    }
                }
            }
        }
    }

    for (EntityId entity_id = it->entity_id + 1; entity_id < MAX_ENTITIES; entity_id++) {
        int matches_all = 1;

        if (it->component_id_filter != 0) {
            for (ComponentId component_id = 0; component_id < COMPONENT_ID_SIZE; component_id++) {
                if (get_requires_component(it->component_id_filter, component_id) &&
                    it->engine->es_memory.components[component_id][entity_id].free) {
                    //no component match. Try the next entity
                    matches_all = 0;
                    break;
                }
            }
        }
        if (it->search_mode == SEARCHMODE_DIST || it->search_mode == SEARCHMODE_DIST_IGNORE_Z) {
            if (it->engine->es_memory.components[COMP_LOCATION][entity_id].free) {
                //no gridlocation, try next entity
                matches_all = 0;
            } else {
                LocationComponent* loc = (LocationComponent*) & it->engine->es_memory.components[COMP_LOCATION][entity_id].any_component;
                float dist_square = SQUARE(loc->pos[0] - it->pos[0]) + SQUARE(loc->pos[1] - it->pos[1]);
                if (it->search_mode == SEARCHMODE_DIST) {
                    dist_square += SQUARE(loc->pos[2] - it->pos[2]);
                }
                if (dist_square > it->max_distance_squared) {
                    //not close enough
                    matches_all = 0;
                    continue;
                }
            }
        }

        //all match, return entity
        if (matches_all) {
            it->entity_id = entity_id;
            if (cache_miss_check) {
                fprintf(stderr, "Cache miss, while an entity DOES match. Location: %f,%f,%f radius %f\n",
                        it->pos[0], it->pos[1], it->pos[2], sqrtf(it->max_distance_squared));
            }
            return 1;
        }
    }
    return 0;
}
