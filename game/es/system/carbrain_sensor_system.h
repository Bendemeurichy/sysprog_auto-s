#ifndef CARBRAIN_VISION_SYSTEM_H
#define CARBRAIN_VISION_SYSTEM_H

#include <stdint.h>
#include <glmc.h>

#include "../engine.h"
#include "../../carbrain/carbrain_io.h"

#define STEP_SIZE 1.0f/3.0f

void system_carbrain_sensor_update(Engine*);

SensorReading sense_environment(Engine *engine, EntityId carbrain_entity_id);
uint8_t sense_pos(Engine *engine, t_vec3 pos);
void set_search_pos(Direction carbrain_direction, t_vec3 pos, t_vec3 car_pos, size_t sensor_index);
int crates(Engine* engine,EntityId car);

#endif //CARBRAIN_VISION_SYSTEM_H
