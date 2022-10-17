#include <stdlib.h>
#include "../carbrain_es_integration.h"

bool is_carbrain_available() {
    return false;
}

void free_CarBrainComponent(CarBrainComponent* carBrainComponent) {
    //do nothing in part 1
}

void
init_CarBrainComponent(CarBrainComponent *carBrainComponent, unsigned int level_nr, unsigned int carbrain_index) {
    //do nothing in part 1
}



int is_carbrain_decision_available(CarBrainComponent *carBrainComponent) {
    //do nothing in part 1
    return 0;
}

CarBrainDecision pop_carbrain_decision(CarBrainComponent* carBrainComponent) {
    //do nothing in part 1
    return 0;
}

void set_carbrain_sense(CarBrainComponent* carBrainComponent, SensorReading* sensor_reading) {
    //do nothing
}

void run_carbrain(CarBrainComponent* carBrainComponent) {
    //do nothing
}
