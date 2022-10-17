#ifndef SYSPROG_CARBRAIN_ES_INTEGRATION_H
#define SYSPROG_CARBRAIN_ES_INTEGRATION_H

#include <stdint.h>
#include <glmc.h>

#include "carbrain_io.h"

#ifdef __cplusplus

extern "C" {
#endif

#include "../es/game_util.h"
#include "../es/component_structs.h"

bool is_carbrain_available();

void free_CarBrainComponent(CarBrainComponent* carBrainComponent);
void init_CarBrainComponent(CarBrainComponent *carBrainComponent, unsigned int level_nr, unsigned int carbrain_index);

int is_carbrain_decision_available(CarBrainComponent* carBrainComponent);
CarBrainDecision pop_carbrain_decision(CarBrainComponent* carBrainComponent);
void set_carbrain_sense(CarBrainComponent* carBrainComponent, SensorReading* sensor_reading);
void run_carbrain(CarBrainComponent* carBrainComponent);


#ifdef __cplusplus
}
#endif

#endif //SYSPROG_CARBRAIN_ES_INTEGRATION_H
