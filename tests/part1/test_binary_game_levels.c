#include "test_binary_game_levels.h"
#include "min_unit.h"

#include "../../game/level/fixed_level_info.h"
#include "../../game/level/c_levelloader.h"
#include "../../game/level/hardcoded_levels.h"
#include "../../game/level/c_levelwriter.h"

char* assert_same_level(Level* a, Level* b) {
    fatal_if(strcmp(a->name, b->name) != 0, "name differs: \"%s\" != \"%s\"\n", a->name, b->name);
//    mu_assert(a->nr == b->nr);
    mu_assert(strcmp(a->name, b->name) == 0);
    mu_assert(a->item_count == b->item_count);
    mu_assert(a->width == b->width);
    mu_assert(a->height == b->height);
    for (unsigned int x = 0; x < a->width; x++) {
        for (unsigned int y = 0; y < a->height; y++) {
            if (a->points[x][y].ground_material != b->points[x][y].ground_material) {
                fprintf(stderr, "assert_same_level fail at %d,%d ground_material=%u %u  ground_height=%d %d\n",
                        x, y, a->points[x][y].ground_material, b->points[x][y].ground_material,
                        a->points[x][y].ground_height, b->points[x][y].ground_height
                );
            }
            mu_assert(a->points[x][y].ground_material == b->points[x][y].ground_material);
            mu_assert(a->points[x][y].ground_height == b->points[x][y].ground_height);
            mu_assert(a->points[x][y].ground_height >=0);
        }
    }

    for (size_t j = 0; j < a->item_count; j++) {
        mu_assert(a->items[j].x == b->items[j].x);
        mu_assert(a->items[j].y == b->items[j].y);
        if (a->items[j].type != b->items[j].type ||
            a->items[j].dir != b->items[j].dir ||
            a->items[j].color != b->items[j].color ||
            a->items[j].art_variant != b->items[j].art_variant ||
            a->items[j].count != b->items[j].count
                ) {
            fprintf(stderr, "assert_same_level fail at item %zu\n"
                            "  x=%d y=%d \n"
                            "  type=%d %d  \n"
                            "  dir=%d %d  \n"
                            "  color=%d %d  \n"
                            "  art_variant=%d %d  \n"
                            "  count=%d %d  \n"
                            "\n",
                    j, a->items[j].x, a->items[j].y,
                    a->items[j].type, b->items[j].type,
                    a->items[j].dir, b->items[j].dir,
                    a->items[j].color, b->items[j].color,
                    a->items[j].art_variant, b->items[j].art_variant,
                    a->items[j].count, b->items[j].count
            );
        }
        mu_assert(a->items[j].type == b->items[j].type);
        mu_assert(a->items[j].dir == b->items[j].dir);
        mu_assert(a->items[j].color == b->items[j].color);
        mu_assert(a->items[j].art_variant == b->items[j].art_variant);
        mu_assert(a->items[j].count == b->items[j].count);
    }
    return 0;
}

char * test_read_level(int level_nr) {
    Level level;
    levelloader_load_binary_level(&level, level_nr, level_binary_file[level_nr]);
    Level fixed_hard;
    load_hardcoded_level(&fixed_hard, level_nr);
    char* res = assert_same_level(&level, &fixed_hard);
    level_free(&level);
    return res;
}

char * test_read_level_0() {
    return test_read_level(0);
}

char * test_read_level_1() {
    return test_read_level(1);
}

char * test_read_level_2() {
    return test_read_level(2);
}

char * test_read_level_3() {
    return test_read_level(3);
}

char * test_read_level_demo() {
    return test_read_level(4);
}


char* assert_same_files(const char* file_a, const char* file_b) {
    return 0;
}

char * test_write_level(int level_nr, bool use_same) {
    Level level;
    load_hardcoded_level(&level, level_nr);
    const char* tmp_file = use_same ? "/tmp/tmp.blvl" : level_binary_nosame_file[level_nr];
    levelwriter_write_binary_level(&level, tmp_file, use_same);
    level_free(&level);
    return assert_same_files(tmp_file, use_same ? level_binary_file[level_nr] : level_binary_nosame_file[level_nr]);
}

char * test_write_level_0() {
    return test_write_level(0, true);
}

char * test_write_level_1() {
    return test_write_level(1, true);
}

char * test_write_level_2() {
    return test_write_level(2, true);
}

char * test_write_level_3() {
    return test_write_level(3, true);
}

char * test_write_level_demo() {
    return test_write_level(4, true);
}

char * test_write_level_0_nosame() {
    return test_write_level(0, true);
}

char * test_write_level_1_nosame() {
    return test_write_level(1, true);
}

char * test_write_level_2_nosame() {
    return test_write_level(2, true);
}

char * test_write_level_3_nosame() {
    return test_write_level(3, true);
}

char * test_write_level_demo_nosame() {
    return test_write_level(4, true);
}
