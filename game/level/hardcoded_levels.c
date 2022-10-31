#include "hardcoded_levels.h"

#include "hardcoded_level_0.hardcoded_level"
#include "hardcoded_level_1.hardcoded_level"
#include "hardcoded_level_2.hardcoded_level"
#include "hardcoded_level_3.hardcoded_level"
#include "hardcoded_level_4.hardcoded_level"

void load_hardcoded_level(Level* level, int level_nr) {
    assert(level_nr >= 0);
    assert(level_nr < TOTAL_LEVEL_COUNT+1);
    switch (level_nr) {
        case 0: { load_hardcoded_level_0(level); break; }
        case 1: { load_hardcoded_level_1(level); break; }
        case 2: { load_hardcoded_level_2(level); break; }
        case 3: { load_hardcoded_level_3(level); break; }
        case 4: { load_hardcoded_level_4(level); break; } //demo level is number 4 internally
        default:
            fatal("Bad level nr %d", level_nr);
    }
}

bool is_hardcoded_level(Level* level) {
    for (int level_nr = 0; level_nr <= TOTAL_LEVEL_COUNT; level_nr++) {
        Level tmp;
        load_hardcoded_level(&tmp, level_nr);
        //Note: This uses ==, which compares pointers, instead of strcmp which compares string content
        //      This is correct in this case! This is the very trick that recognizes hardcoded levels.
        if (tmp.name == level->name) {
            return true;
        }
    }
    return false;
}
