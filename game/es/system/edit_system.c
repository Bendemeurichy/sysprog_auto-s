// #include "move_system.h"
#include "edit_system.h"
#include "../assemblage.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>


void system_edit_update(Engine* engine) {
    //TODO process COMP_EDIT_REQUEST. (you will also need COMP_EDITOR_TOOLBAR). 
    //     If an edit was made, set redo_art to true
    bool redo_art = false; //TODO set to true if any TileEditInfoComponent was changed

    if (redo_art) {
        //an edit was made, so change reset art entities based on the updated TileEditInfoComponent
        Level level;
        leveledit_entities_to_level(engine, &level);
        clear_level(engine, true);
        set_level_entities(engine, &level, true, false, false);

        //don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
        update_gridloc_cache(&engine->es_memory);
    }
}