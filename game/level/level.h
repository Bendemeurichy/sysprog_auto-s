#ifndef LEVEL_H
#define LEVEL_H

#include <glmc.h>
#include "../es/game_util.h"
#include "../graphics/renderer_3d.h"

typedef enum LevelItemColor {
    LEVEL_COLOR_GREY, LEVEL_COLOR_RAINBOW,
    LEVEL_COLOR_0, LEVEL_COLOR_1, LEVEL_COLOR_2, LEVEL_COLOR_3, LEVEL_COLOR_4
} LevelItemColor;

typedef struct LevelPoint {
    Material ground_material;
    int ground_height;
} LevelPoint;

typedef enum LevelItemType {
    CAR, EXIT, FILTER, CRATE, DECORATION, BUILDING, MARKER
} LevelItemType;

typedef struct LevelItem {
    int x;
    int y;

    LevelItemType type;
    Direction dir;  //buildings, cars and exits point a certain direction (N, S, E, W)
    LevelItemColor color; //only for crates
    uint8_t art_variant; //for obstacles
    uint8_t count; //building have a story count, and filters have a crate count
} LevelItem;

typedef struct Level {
    int nr;
    char* name;
    char* source_file_name;

    unsigned int width;
    unsigned int height;
    LevelPoint** points;

    LevelItem* items;
    size_t item_count;
} Level;

void level_init(Level *level, int width, int height, size_t item_count, int nr, const char *name, const char* source_file_name);
void level_free(Level *level);

const char* level_item_type_str(LevelItemType in);
const char* level_item_color_str(LevelItemColor in);

#endif //LEVEL_H
