#include "levelloader_system.h"
#include "../assemblage.h"
#include "../../level/c_levelwriter.h"

#include <stdio.h>

void system_levelloader_update(Engine* engine) {
    WorldComponent* world = search_first_component(engine, COMP_WORLD);
    LevelInfoComponent* level_info = search_first_component(engine, COMP_LEVEL_INFO);

    if (world->game_state == GAME_EDIT) {
        //TODO extend this basic editor levelloader code
        //     see assignment for info on what levelloader needs to do with R, F5, and F9 keys and filenames.
        EditorInfoComponent* editor_info = search_first_component(engine, COMP_EDITOR_INFO);

        if (world->game_request == GAME_REQUEST_INIT
            || world->game_request == GAME_REQUEST_RESTART_LEVEL
            || world->game_request == GAME_REQUEST_LOAD) {
            world->game_state = GAME_EDIT;
            Level *new_level = &engine->levels[0];
            if(world->game_request==GAME_REQUEST_LOAD){
            
            }
            clear_level(engine, false);
            set_level_entities(engine, new_level, true, true, false);
            update_gridloc_cache(&engine->es_memory);
        }
        if (world->game_request == GAME_REQUEST_SAVE) {
            Level level;
            leveledit_entities_to_level(engine, &level);
            //TODO set edited_level_filename based on editor_info->level_base_filename
            const char* edited_level_filename = strcat(editor_info->level_base_filename,"_edited.blvl");
            levelwriter_write_binary_level(&level, edited_level_filename, true);
        }
        world->game_request = GAME_REQUEST_NONE;
    } else {
        if (world->game_request == GAME_REQUEST_INIT || world->game_request == GAME_REQUEST_GOTO_LEVEL ||
            world->game_request == GAME_REQUEST_NEXT_LEVEL || world->game_request == GAME_REQUEST_PREV_LEVEL ||
            world->game_request == GAME_REQUEST_RESTART_LEVEL) {
            if (world->game_request == GAME_REQUEST_INIT) {
                level_info->nr = world->initial_level_nr;
            }
            if (world->game_request == GAME_REQUEST_NEXT_LEVEL) {
                level_info->nr++;
                if (level_info->nr >= engine->level_count) {
                    level_info->nr = engine->level_count -1;
                    world->game_state = GAME_WIN;
                    world->game_request = GAME_REQUEST_NONE;
                    update_gridloc_cache(&engine->es_memory);
                    return;
                }
            }
            if (world->game_request == GAME_REQUEST_PREV_LEVEL) {
                if (level_info->nr <= 0) {
                    world->game_request = GAME_REQUEST_NONE;
                    return;
                } else {
                    level_info->nr--;
                }
            }
            if (world->game_request == GAME_REQUEST_GOTO_LEVEL) {
                level_info->nr = world->requested_level_nr;
                if (level_info->nr >= engine->level_count) {
                    level_info->nr = engine->level_count-1;
                    world->game_state = GAME_WIN;
                    world->game_request = GAME_REQUEST_NONE;
                    update_gridloc_cache(&engine->es_memory);
                    return;
                }
            }
            world->game_state = GAME_PLAYING;
            assert(level_info->nr >= 0);
            assert(level_info->nr < engine->level_count);
            Level* new_level = &engine->levels[level_info->nr];

            clear_level(engine, false);
            update_gridloc_cache(&engine->es_memory);

            set_level_entities(engine, new_level, false, false, true);
            //printf(
            //        "Loaded level %d (\"%s\"), there are now %d entities in use\n",
            //        level_info->nr,
            //        new_level->name,
            //        entity_used_count(engine)
            //);

            update_gridloc_cache(&engine->es_memory);
        }
        world->game_request = GAME_REQUEST_NONE;
    }
}
