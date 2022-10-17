#ifndef ENGINE_H
#define ENGINE_H

typedef struct Engine Engine;

#include "entity.h"
#include "system/render_system.h"
#include "system/input_system.h"
#include <stdint.h>
#include <glmc.h>

#include "es_memory_manager_collections.h"

#include "es_memory_manager.h"
#include "../graphics/renderer_3d.h"
#include "../level/level.h"

typedef struct Engine {
    //Only 2 things are stored outside of the entity framework system:
    // - Fixed info and FPS info for the render system.
    RenderStaticInfo* render_static_info;

    // - The list of levels, which never changes after init, and is needed in a few places, such as the levelloader system.
    Level* levels;
    size_t level_count;

    //Storage for all components
    ESMemory es_memory;

    uint64_t previous_tick;
} Engine;

void engine_init(Engine*, Renderer3D*);
void engine_free(Engine*);

void engine_update(Engine*);

#endif //ENGINE_H
