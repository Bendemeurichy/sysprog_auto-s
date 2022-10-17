#ifndef ASSEMBLAGE_H
#define ASSEMBLAGE_H

#include <glmc.h>
#include "game_util.h"
#include "engine.h"

void create_base_entities(Engine *engine);

void set_level_entities(Engine* engine, Level *level,
                        bool set_editor_info, bool create_editor_entities, bool create_game_entities);
void clear_level(Engine* engine, bool clear_art_only);

void leveledit_entities_to_level(Engine* engine, Level *level);

void level_pos_to_entity_pos(int x, int y, int z, t_vec3 res, int middle);

#endif
