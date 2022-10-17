#ifndef CARBRAIN_VISION_SYSTEM_H
#define CARBRAIN_VISION_SYSTEM_H

#include <stdint.h>
#include <glmc.h>

#include "../engine.h"
#include "../../carbrain/carbrain_io.h"

void system_carbrain_sensor_update(Engine*);

SensorReading sense_environment(Engine *engine, EntityId carbrain_entity_id);
void set_search_pos(Direction carbrain_direction, t_vec3 pos, size_t sensor_index);

#endif //CARBRAIN_VISION_SYSTEM_H
