#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "../engine.h"

#define OBJECT_FALL_MS 10u
#define OBJECT_FALL_DIST 0.05f
#define PLAYER_MOVE_MS 500u
#define DOOR_MOVE_MS 200u

int do_blockers_overlap__excluding_edge(Engine* engine, EntityId a, EntityId b);
int is_point_in_blocker__excluding_edge(Engine* engine, t_vec3 point, EntityId a);

bool is_destination_blocked(Engine* engine, EntityId moving_entity_id, t_vec3 dest);

void system_move_update(Engine*);

#endif //MOVE_SYSTEM_H
