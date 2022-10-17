#ifndef SYSPROG_ENGINE_COLLECTIONS_H
#define SYSPROG_ENGINE_COLLECTIONS_H

typedef struct ComponentIterator ComponentIterator;
typedef struct EntityList EntityList;
typedef struct EntityIterator EntityIterator;
typedef struct Engine Engine;

#include "entity.h"
#include "component_enums.h"

#include <stdint.h>
#include <glmc.h>

#define NO_ENTITY (0xFFFFFFFF)

#define MAX_CACHE_DISTANCE_SQUARE 1.0f

typedef struct ComponentIterator {
    Engine* engine;
    EntityId entity_id;
    ComponentId component_id;
    void* comp;
} ComponentIterator;

void search_component(Engine* engine,
                      ComponentId component_id,
                      ComponentIterator* res);
int next_component(ComponentIterator*);

//returns a pointer to the component
void* search_first_component(Engine* engine, ComponentId component_id);

typedef uint64_t component_id_filter_t;

typedef enum SearchMode { SEARCHMODE_ANY, SEARCHMODE_DIST, SEARCHMODE_DIST_IGNORE_Z } SearchMode;
typedef struct EntityIterator {
    Engine* engine;
    EntityId entity_id;
    component_id_filter_t component_id_filter;
    SearchMode search_mode; //mode: SEARCHMODE_ANY->ignore pos  SEARCHMODE_DIST->require match of pos  SEARCHMODE_DIST_IGNORE_Z->require match on pos X and Y but ignore Z
    float max_distance_squared;
    t_vec3 pos;
    unsigned int pos_cache_index;
} EntityIterator;


component_id_filter_t set_requires_component(component_id_filter_t component_id_filter, ComponentId component_id);

void search_entity(Engine *engine, component_id_filter_t component_id_filter, EntityIterator *res);
int next_entity(EntityIterator*);

void search_entity_1(Engine *engine, ComponentId component_id1, EntityIterator *res);
void search_entity_2(Engine *engine, ComponentId component_id1, ComponentId component_id2, EntityIterator *res);
void search_entity_3(Engine *engine, ComponentId component_id1, ComponentId component_id2, ComponentId component_id3,
                     EntityIterator *res);

void
search_entity_by_location_1(Engine *engine, t_vec3 pos, float max_distance, int ignore_z, ComponentId component_id1,
                            EntityIterator *res);

void
search_entity_by_location_2(Engine *engine, t_vec3 pos, float max_distance, int ignore_z, ComponentId component_id1,
                            ComponentId component_id2, EntityIterator *res);

EntityId search_first_entity_with_mask(Engine *engine, component_id_filter_t component_id_filter);
EntityId search_first_entity_1(Engine *engine, ComponentId component_id);
EntityId search_first_entity_2(Engine *engine, ComponentId component_id, ComponentId component_id2);
EntityId search_first_entity_3(Engine *engine, ComponentId component_id, ComponentId component_id2, ComponentId component_id3);


#endif //SYSPROG_ENGINE_COLLECTIONS_H
