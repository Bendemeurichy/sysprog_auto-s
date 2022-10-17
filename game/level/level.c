#include "level.h"
#include "../../../util/util.h"
#include "hardcoded_levels.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

void level_init(Level *level, int width, int height, size_t item_count, int nr, const char *name, const char* source_file_name) {
    assert(level != NULL);

    level->nr = nr;

    assert(name);
    level->name = calloc(strlen(name)+1, sizeof(char));
    strcpy(level->name, name);

    assert(source_file_name != NULL);
    level->source_file_name = calloc(strlen(source_file_name) + 1, sizeof(char));
    strcpy(level->source_file_name, source_file_name);

    level->item_count = item_count;
    level->items = calloc(item_count, sizeof(LevelItem));
    
    level->width = width;
    level->height = height;
    level->points = calloc(level->width, sizeof(LevelPoint*));
    
    for (int x = 0; x < level->width; x++) {
        level->points[x] = calloc(level->height, sizeof(LevelPoint));
        for (int y = 0; y < level->height; y++) {
            level->points[x][y].ground_material = ART_WATER;
            level->points[x][y].ground_height = 0;
        }
    }
}
void level_free(Level *level) {
    if (!is_hardcoded_level(level)) {
        for (int x = 0; x < level->width; x++) {
            free(level->points[x]);
        }
        free(level->points);
        free(level->items);
        free(level->name);
        free(level->source_file_name);
    }
    
    level->points = NULL;
    level->items = NULL;
    level->name = NULL;
    level->width = 0;
    level->height = 0;
    level->item_count = 0;
}

const char* level_item_type_str(LevelItemType in) {
    switch(in) {
        case CAR: return "CAR";
        case EXIT: return "EXIT";
        case FILTER: return "FILTER";
        case CRATE: return "CRATE";
        case DECORATION: return "DECORATION";
        case BUILDING: return "BUILDING";
        case MARKER: return "MARKER";
        default: { fatal("Unsupported LevelItemType %d", in); return NULL; }
    }
}

const char* level_item_color_str(LevelItemColor in) {
    switch(in) {
        case LEVEL_COLOR_GREY: return "LEVEL_COLOR_GREY";
        case LEVEL_COLOR_RAINBOW: return "LEVEL_COLOR_RAINBOW";
        case LEVEL_COLOR_0: return "LEVEL_COLOR_0";
        case LEVEL_COLOR_1: return "LEVEL_COLOR_1";
        case LEVEL_COLOR_2: return "LEVEL_COLOR_2";
        case LEVEL_COLOR_3: return "LEVEL_COLOR_3";
        case LEVEL_COLOR_4: return "LEVEL_COLOR_4";
        default: { fatal("Unsupported LevelItemColor %d", in); return NULL; }
    }
}
