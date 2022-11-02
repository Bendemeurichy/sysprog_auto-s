#ifndef SYSPROG_GAME_UTIL_H
#define SYSPROG_GAME_UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <glmc.h>
#include <math.h>

#include "../../util/util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Direction definition:
 *
 *        OpenGL           game grid       related game width or height
 * N -> towards min Z -> towards max y  -> height
 * S -> towards max Z -> towards min y  -> height
 * E -> towards max X -> towards max x  -> width
 * W -> towards min X -> towards min x  -> width
 * */

typedef enum Direction { N, W, S, E, DIRECTION_COUNT } Direction;

#define DIRECTION_FIRST N
#define DIRECTION_LAST E

static_assert(N == 0, "North must be 0");
static_assert(W == 1, "West must be 1");
static_assert(S == 2, "South must be 2");
static_assert(E == 3, "East must be 3");
static_assert(DIRECTION_COUNT == 4, "DIRECTION_COUNT must be 4");
static_assert(DIRECTION_FIRST == 0, "DIRECTION_FIRST must be 0");
static_assert(DIRECTION_LAST == DIRECTION_COUNT-1, "DIRECTION_LAST must be last");

#define DIR_TO_CHAR(dir) (dir == N ? "N" : (dir == W ? "W" : (dir == S ? "S" : (dir == E ? "E" : "0"+dir))))
#define is_direction(in) ((in) >= 0 && (in) < DIRECTION_COUNT)

//#define DIR_MASK 0x3u
//#define OPPOSITE_DIRECTION_2D(DIR) ((((DIR) & DIR_MASK) + 2) & DIR_MASK)
//#define LEFT_DIRECTION_2D(DIR) ((((DIR) & DIR_MASK) + 1) & DIR_MASK)
//#define RIGHT_DIRECTION_2D(DIR) ((((DIR) & DIR_MASK) + (DIRECTION_COUNT-1)) & DIR_MASK)
#define OPPOSITE_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + 2) % DIRECTION_COUNT)
#define LEFT_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + 1) % DIRECTION_COUNT)
#define RIGHT_DIRECTION_2D(DIR) ((((DIR) % DIRECTION_COUNT) + (DIRECTION_COUNT-1)) % DIRECTION_COUNT)

#define DIRECTION_INC(DIRVAR) DIRVAR = (Direction)(((uint8_t)DIRVAR)+1)

#define DIRECTION_TO_NORTH_FLOAT_ANGLE_RADIANS_2D(DIR) (((DIR) % DIRECTION_COUNT)*M_PI_2)

#define ALL_DIRECTION_ARRAY_CONDITION(ARR, COND) ((ARR[N] COND) && (ARR[W] COND) && (ARR[S] COND) && (ARR[E] COND))
#define ANY_DIRECTION_ARRAY_CONDITION(ARR, COND) ((ARR[N] COND) || (ARR[W] COND) || (ARR[S] COND) || (ARR[E] COND))

static_assert(OPPOSITE_DIRECTION_2D(N) == S, "OPPOSITE_DIRECTION_2D(N)");
static_assert(OPPOSITE_DIRECTION_2D(W) == E, "OPPOSITE_DIRECTION_2D(W)");
static_assert(OPPOSITE_DIRECTION_2D(S) == N, "OPPOSITE_DIRECTION_2D(S)");
static_assert(OPPOSITE_DIRECTION_2D(E) == W, "OPPOSITE_DIRECTION_2D(E)");

static_assert(LEFT_DIRECTION_2D(N) == W, "LEFT_DIRECTION_2D(N)");
static_assert(LEFT_DIRECTION_2D(W) == S, "LEFT_DIRECTION_2D(W)");
static_assert(LEFT_DIRECTION_2D(S) == E, "LEFT_DIRECTION_2D(S)");
static_assert(LEFT_DIRECTION_2D(E) == N, "LEFT_DIRECTION_2D(E)");

static_assert(RIGHT_DIRECTION_2D(N) == E, "RIGHT_DIRECTION_2D(N)");
static_assert(RIGHT_DIRECTION_2D(E) == S, "RIGHT_DIRECTION_2D(E)");
static_assert(RIGHT_DIRECTION_2D(S) == W, "RIGHT_DIRECTION_2D(S)");
static_assert(RIGHT_DIRECTION_2D(W) == N, "RIGHT_DIRECTION_2D(W)");

#define RELATIVE_X(DIR, X) ((DIR) == E ? ((X)+1) : ((DIR) == W ? ((X)-1) : (X)))
#define RELATIVE_Y(DIR, Y) ((DIR) == N ? ((Y)+1) : ((DIR) == S ? ((Y)-1) : (Y)))

static_assert(RELATIVE_X(N, 1) == 1, "RELATIVE_X(N, 1)");
static_assert(RELATIVE_X(S, 1) == 1, "RELATIVE_X(S, 1)");
static_assert(RELATIVE_X(E, 1) == 2, "RELATIVE_X(E, 1)");
static_assert(RELATIVE_X(W, 1) == 0, "RELATIVE_X(W, 1)");

static_assert(RELATIVE_Y(N, 1) == 2, "RELATIVE_Y(N, 1)");
static_assert(RELATIVE_Y(S, 1) == 0, "RELATIVE_Y(S, 1)");
static_assert(RELATIVE_Y(E, 1) == 1, "RELATIVE_Y(E, 1)");
static_assert(RELATIVE_Y(W, 1) == 1, "RELATIVE_Y(W, 1)");


void get_relative_position_ivec3(t_ivec3 orig_pos, Direction dir, t_ivec3 target_pos);
void get_relative_position_vec3(t_vec3 orig_pos, Direction dir, t_vec3 target_pos);


typedef enum Direction8 { N8, NE8, E8, SE8, S8, SW8, W8, NW8, DIRECTION8_COUNT } Direction8;

#define DIRECTION8_FIRST N8
#define DIRECTION8_LAST NW8

#define RELATIVE8_X(DIR, X) (((DIR) == E8 || (DIR) == SE8 || (DIR) == NE8) ? ((X)+1) : ((DIR) == W8 || (DIR) == NW8 || (DIR) == SW8 ? ((X)-1) : (X)))
#define RELATIVE8_Y(DIR, Y) ((DIR) == N8 || (DIR) == NE8 || (DIR) == NW8 ? ((Y)+1) : ((DIR) == S8 || (DIR) == SW8 || (DIR) == SE8 ? ((Y)-1) : (Y)))

static_assert(RELATIVE8_X(N8, 1) == 1, "RELATIVE8_X(N, 1)");
static_assert(RELATIVE8_X(NE8, 1) == 2, "RELATIVE8_X(NE, 1)");
static_assert(RELATIVE8_X(E8, 1) == 2, "RELATIVE8_X(E, 1)");
static_assert(RELATIVE8_X(SE8, 1) == 2, "RELATIVE8_X(SE, 1)");
static_assert(RELATIVE8_X(S8, 1) == 1, "RELATIVE8_X(S, 1)");
static_assert(RELATIVE8_X(SW8, 1) == 0, "RELATIVE8_X(SW, 1)");
static_assert(RELATIVE8_X(W8, 1) == 0, "RELATIVE8_X(W, 1)");
static_assert(RELATIVE8_X(NW8, 1) == 0, "RELATIVE8_X(NW, 1)");

static_assert(RELATIVE8_Y(N8, 1) == 2, "RELATIVE8_Y(N, 1)");
static_assert(RELATIVE8_Y(NE8, 1) == 2, "RELATIVE8_Y(NE, 1)");
static_assert(RELATIVE8_Y(E8, 1) == 1, "RELATIVE8_Y(E, 1)");
static_assert(RELATIVE8_Y(SE8, 1) == 0, "RELATIVE8_Y(SE, 1)");
static_assert(RELATIVE8_Y(S8, 1) == 0, "RELATIVE8_Y(S, 1)");
static_assert(RELATIVE8_Y(SW8, 1) == 0, "RELATIVE8_Y(SW, 1)");
static_assert(RELATIVE8_Y(W8, 1) == 1, "RELATIVE8_Y(W, 1)");
static_assert(RELATIVE8_Y(NW8, 1) == 2, "RELATIVE8_Y(NW, 1)");

#define RELATIVE8F_X(DIR, X) (((DIR) == E8 || (DIR) == SE8 || (DIR) == NE8) ? ((X)+(1.0f/3.0f)) : ((DIR) == W8 || (DIR) == NW8 || (DIR) == SW8 ? ((X)-(1.0f/3.0f)) : (X)))
#define RELATIVE8F_Y(DIR, Y) ((DIR) == N8 || (DIR) == NE8 || (DIR) == NW8 ? ((Y)+(1.0f/3.0f)) : ((DIR) == S8 || (DIR) == SW8 || (DIR) == SE8 ? ((Y)-(1.0f/3.0f)) : (Y)))

#define direction8_is_90(in) ((in) == N8 || (in) == S8 || (in) == E8 || (in) == W8)
#define is_direction8(in) ((in) >= 0 && (in) < DIRECTION8_COUNT)
#define direction8_to_direction(in) (((in)==N8?N:(in)==E8?E:(in)==S8?S:(in)==W8?W:0xFF))
#define direction_to_direction8(in) (((in)==N?N8:(in)==E?E8:(in)==S?S8:(in)==W?W8:0xFF))

static_assert(direction8_to_direction(N8) == N, "direction8_to_direction(N8) == N");
static_assert(direction8_to_direction(E8) == E, "direction8_to_direction(E8) == E");
static_assert(direction8_to_direction(S8) == S, "direction8_to_direction(S8) == S");
static_assert(direction8_to_direction(W8) == W, "direction8_to_direction(W8) == W");

static_assert(direction_to_direction8(N) == N8, "direction_to_direction8(N) == N8");
static_assert(direction_to_direction8(E) == E8, "direction_to_direction8(E) == E8");
static_assert(direction_to_direction8(S) == S8, "direction_to_direction8(S) == S8");
static_assert(direction_to_direction8(W) == W8, "direction_to_direction8(W) == W8");

#define DIR8_STR(X) ((X) == N8 ? "N" : (X) == NE8 ? "NE" : (X) == E8 ? "E" : (X) == SE8 ? "SE" : (X) == S8 ? "S" : (X) == SW8 ? "SW" : (X) == W8 ? "W" : "NW")

#define RIGHT_DIRECTION8_45_2D(DIR8) ((((DIR8) % DIRECTION8_COUNT) + 0) % DIRECTION8_COUNT)
#define LEFT_DIRECTION8_45_2D(DIR8) ((((DIR8) % DIRECTION8_COUNT) + (DIRECTION8_COUNT-1)) % DIRECTION8_COUNT)
#define RIGHT_DIRECTION8_90_2D(DIR8) ((((DIR8) % DIRECTION8_COUNT) + 2) % DIRECTION8_COUNT)
#define LEFT_DIRECTION8_90_2D(DIR8) ((((DIR8) % DIRECTION8_COUNT) + (DIRECTION8_COUNT-2)) % DIRECTION8_COUNT)


// Relative directions use Direction9
// C = CENTER

typedef enum Direction9 { C9, N9, NE9, E9, SE9, S9, SW9, W9, NW9, DIRECTION9_COUNT } Direction9;

#define DIRECTION9_FIRST N9
#define DIRECTION9_LAST NW9

static_assert(N9 == 1, "North must be 1");
static_assert(NW9 == 8, "West must be 8");
static_assert(C9 == 0, "Center must be 0");
static_assert(DIRECTION9_COUNT == 9, "DIRECTION9_COUNT must be 9");

#define RELATIVE9_X(DIR, X) (((DIR) == E9 || (DIR) == SE9 || (DIR) == NE9) ? ((X)+1) : ((DIR) == W9 || (DIR) == NW9 || (DIR) == SW9 ? ((X)-1) : (X)))
#define RELATIVE9_Y(DIR, Y) ((DIR) == N9 || (DIR) == NE9 || (DIR) == NW9 ? ((Y)+1) : ((DIR) == S9 || (DIR) == SW9 || (DIR) == SE9 ? ((Y)-1) : (Y)))

static_assert(RELATIVE9_X(N9, 1) == 1, "RELATIVE9_X(N, 1)");
static_assert(RELATIVE9_X(NE9, 1) == 2, "RELATIVE9_X(NE, 1)");
static_assert(RELATIVE9_X(E9, 1) == 2, "RELATIVE9_X(E, 1)");
static_assert(RELATIVE9_X(SE9, 1) == 2, "RELATIVE9_X(SE, 1)");
static_assert(RELATIVE9_X(S9, 1) == 1, "RELATIVE9_X(S, 1)");
static_assert(RELATIVE9_X(SW9, 1) == 0, "RELATIVE9_X(SW, 1)");
static_assert(RELATIVE9_X(W9, 1) == 0, "RELATIVE9_X(W, 1)");
static_assert(RELATIVE9_X(NW9, 1) == 0, "RELATIVE9_X(NW, 1)");

static_assert(RELATIVE9_Y(N9, 1) == 2, "RELATIVE9_Y(N, 1)");
static_assert(RELATIVE9_Y(NE9, 1) == 2, "RELATIVE9_Y(NE, 1)");
static_assert(RELATIVE9_Y(E9, 1) == 1, "RELATIVE9_Y(E, 1)");
static_assert(RELATIVE9_Y(SE9, 1) == 0, "RELATIVE9_Y(SE, 1)");
static_assert(RELATIVE9_Y(S9, 1) == 0, "RELATIVE9_Y(S, 1)");
static_assert(RELATIVE9_Y(SW9, 1) == 0, "RELATIVE9_Y(SW, 1)");
static_assert(RELATIVE9_Y(W9, 1) == 1, "RELATIVE9_Y(W, 1)");
static_assert(RELATIVE9_Y(NW9, 1) == 2, "RELATIVE9_Y(NW, 1)");

static_assert(RELATIVE9_Y(C9, 1) == 1, "RELATIVE9_Y(C, 1)");
static_assert(RELATIVE9_X(C9, 1) == 1, "RELATIVE9_X(C, 1)");

#define SQUARE(a) ((a)*(a))
#define INTERVAL_OVERLAP__INCLUSIVE(A1, A2, B1, B2) ((A1) <= (B2) && (B1) <= (A2))
#define INTERVAL_DISJOINT__INCLUSIVE(A1, A2, B1, B2) ((A1) > (B2) || (B1) > (A2))
#define INTERVAL_OVERLAP__EXCLUSIVE(A1, A2, B1, B2) ((A1) < (B2) && (B1) < (A2))
#define INTERVAL_DISJOINT__EXCLUSIVE(A1, A2, B1, B2) ((A1) >= (B2) || (B1) >= (A2))

#ifdef __cplusplus
}
#endif

const char* direction_to_string(Direction dir);
const char* direction8_to_string(Direction8 dir);
const char* direction9_to_string(Direction9 dir);

#endif //SYSPROG_GAME_UTIL_H
