#ifndef TEST_BINARY_GAME_LEVELS
#define TEST_BINARY_GAME_LEVELS

#include "../../game/level/level.h"

char* assert_same_level(Level* a, Level* b);

char * test_read_level_0();
char * test_read_level_1();
char * test_read_level_2();
char * test_read_level_3();
char * test_read_level_demo();

char * test_write_level_0();
char * test_write_level_1();
char * test_write_level_2();
char * test_write_level_3();
char * test_write_level_demo();

char * test_write_level_0_nosame();
char * test_write_level_1_nosame();
char * test_write_level_2_nosame();
char * test_write_level_3_nosame();
char * test_write_level_demo_nosame();

#endif  // TEST_BINARY_GAME_LEVELS
