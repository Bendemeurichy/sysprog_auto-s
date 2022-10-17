#ifndef CARBRAIN_DECISION_SYSTEM_H
#define CARBRAIN_DECISION_SYSTEM_H


#include <stdint.h>

#include "../engine.h"
#include "../../carbrain/carbrain_io.h"

void system_carbrain_decision_update(Engine*);

void execute_carbrain_decision(Engine *engine, CarBrainDecision decision, EntityId carbrain_entity_id);

#endif //CARBRAIN_DECISION_SYSTEM_H
