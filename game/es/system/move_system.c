#include "move_system.h"

void system_move_update(Engine* engine) {
    //TODO

    //don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
    update_gridloc_cache(&engine->es_memory);
}
