#ifndef SYSPROG_ES_MEMORY_MANAGER_H
#define SYSPROG_ES_MEMORY_MANAGER_H

#include "entity.h"
#define MAX_ENTITIES (60u*24u*3u)

#include "component_structs.h"
#include <assert.h>

typedef struct AllComponent {
    int free;

    //this union is used to make AllComponent large enough to store any of the components.
    union {
        //make sure each and every component is in here! (although there is no problem as long as you don't forget the component with the largest "sizeof")
        CameraComponent camera_lookfrom;
        MoveRequestComponent move_request_component;
        LocationComponent location_component;
        MoveComponent moveanimation_component;
        CarArtComponent car_art_component;
        GroundArtComponent floor_art_component;
        FilterComponent filter_component;
        InputReceiverComponent inputreceiver_component;
        DirectionComponent direction_component;
        UpDownComponent updown_component;
        WorldComponent world_component;
        LevelInfoComponent level_info_component;
        EditorInfoComponent editor_info_component;
        DraggableComponent draggable_component;
        DraggingComponent dragging_component;
        ExitedComponent exited_component;
        CarBrainComponent carbrain_component;
        TileEditInfoComponent tile_edit_info;
    } any_component;
} AllComponent;

#define CACHE_X_TRANSFORM(X) ((int)floorf(X))
#define CACHE_Y_TRANSFORM(X) ((int)floorf(X))

#define GRIDLOC_CACHE_MIN_X -1
#define GRIDLOC_CACHE_MIN_Y -1
#define GRIDLOC_CACHE_MAX_X 61
#define GRIDLOC_CACHE_MAX_Y 25
#define GRIDLOC_CACHE_DIM_X (1 + (GRIDLOC_CACHE_MAX_X - GRIDLOC_CACHE_MIN_X))
#define GRIDLOC_CACHE_DIM_Y (1 + (GRIDLOC_CACHE_MAX_Y - GRIDLOC_CACHE_MIN_Y))
#define GRIDLOC_CACHE_MAX_PER_CELL 10

#define GRIDLOC_CACHE_TOO_MANY -1

typedef struct ESMemory {
    EntityId next_entity_id;
    
    AllComponent components[COMPONENT_ID_SIZE][MAX_ENTITIES];
    
    EntityId gridloc_cache[GRIDLOC_CACHE_DIM_X][GRIDLOC_CACHE_DIM_Y][GRIDLOC_CACHE_MAX_PER_CELL];
    int gridloc_cache_count[GRIDLOC_CACHE_DIM_X][GRIDLOC_CACHE_DIM_Y];
} ESMemory;

void es_memory_manager_init(ESMemory*);
void es_memory_manager_free(ESMemory*);

void update_gridloc_cache(ESMemory* mem);

#include "engine.h"

int has_component(Engine* engine, EntityId entity_id, ComponentId component_id);
void* get_component(Engine* engine, EntityId entity_id, ComponentId component_id);

void* create_component(Engine* engine, EntityId entity_id, ComponentId component_id);
void* create_component_if_not_present(Engine* engine, EntityId entity_id, ComponentId component_id);
void free_component(Engine* engine, EntityId entity_id, ComponentId component_id);
void free_component_if_present(Engine* engine, EntityId entity_id, ComponentId component_id);

EntityId get_new_entity_id(Engine* engine);
EntityId get_new_entity_with_id(Engine* engine, EntityId entity_id);
int is_entity_free(Engine* engine, EntityId entity_id);
void delete_entity(Engine* engine, EntityId entity_id);
int entity_used_count(Engine* engine);

#endif //SYSPROG_ES_MEMORY_MANAGER_H
