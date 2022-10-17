#include "game_util.h"

#include <glmc.h>

void get_relative_position_ivec3(t_ivec3 orig_pos, Direction dir, t_ivec3 target_pos) {
    switch (dir) {
        case N: {
            glmc_ivec3_set(target_pos, orig_pos[0], orig_pos[1] + 1, orig_pos[2]);
            break;
        }
        case S: {
            glmc_ivec3_set(target_pos, orig_pos[0], orig_pos[1] - 1, orig_pos[2]);
            break;
        }
        case E: {
            glmc_ivec3_set(target_pos, orig_pos[0] + 1, orig_pos[1], orig_pos[2]);
            break;
        }
        case W: {
            glmc_ivec3_set(target_pos, orig_pos[0] - 1, orig_pos[1], orig_pos[2]);
            break;
        }
        default:
            fatal("Bad direction");
    }
}

void get_relative_position_vec3(t_vec3 orig_pos, Direction dir, t_vec3 target_pos) {
    switch (dir) {
        case N: {
            glmc_vec3_set(target_pos, orig_pos[0], orig_pos[1] + 1.0f, orig_pos[2]);
            break;
        }
        case S: {
            glmc_vec3_set(target_pos, orig_pos[0], orig_pos[1] - 1.0f, orig_pos[2]);
            break;
        }
        case E: {
            glmc_vec3_set(target_pos, orig_pos[0] + 1.0f, orig_pos[1], orig_pos[2]);
            break;
        }
        case W: {
            glmc_vec3_set(target_pos, orig_pos[0] - 1.0f, orig_pos[1], orig_pos[2]);
            break;
        }
        default:
            fatal("Bad direction");
    }
}

const char* direction_to_string(Direction dir) {
    switch (dir) {
        case N: return "N";
        case E: return "E";
        case S: return "S";
        case W: return "W";
        default: return "INVALID DIR";
    }
}
const char* direction8_to_string(Direction8 dir) {
    switch (dir) {
        case N8: return "N8";
        case NE8: return "NE8";
        case E8: return "E8";
        case SE8: return "SE8";
        case S8: return "S8";
        case SW8: return "SW8";
        case W8: return "W8";
        case NW8: return "NW8";
        default: return "INVALID DIR8";
    }
}
const char* direction9_to_string(Direction9 dir) {
    switch (dir) {
        case N9: return "N9";
        case NE9: return "NE9";
        case E9: return "E9";
        case SE9: return "SE9";
        case S9: return "S9";
        case SW9: return "SW9";
        case W9: return "W9";
        case NW9: return "NW9";
        case C9: return "C9";
        default: return "INVALID DIR9";
    }
}