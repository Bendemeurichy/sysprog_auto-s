#include "c_levelloader.h"

#include "fixed_level_info.h"
#include "hardcoded_levels.h"

#include <assert.h>

void levelloader_load_binary_level(Level *level, unsigned int level_nr, const char *filename) {
    //TODO
}

void levelloader_load_fixed_level(Level *level, unsigned int level_nr) {
    assert(level != NULL);
    assert(level_nr >= 0);
    assert(level_nr <TOTAL_LEVEL_COUNT);
    
    //TODO: uncomment the next line
//    levelloader_load_binary_level(level, level_nr, level_binary_file[level_nr]);
    
    //TODO: comment the next line
    load_hardcoded_level(level, level_nr);
}

void levelloader_load_demo_level(Level *level) {
    assert(level != NULL);

    //TODO: uncomment the next line
//    levelloader_load_binary_level(level, DEMO_LEVEL_NR, level_binary_file[DEMO_LEVEL_NR]);

    //TODO: comment the next line
    load_hardcoded_level(level, DEMO_LEVEL_NR);
}
