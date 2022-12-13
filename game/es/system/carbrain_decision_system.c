#include "carbrain_decision_system.h"
#include "../../carbrain/carbrain_io.h"
#include "../../carbrain/carbrain_es_integration.h"

void system_carbrain_decision_update(Engine* engine) {
    //only in part2! You can leave this empty in part1
    //TODO search all carbrains, run their CPUs, check if a decision is available, and execute it if so.

    EntityIterator it;
    search_entity_1(engine,COMP_CARBRAIN , &it);

    while (next_entity(&it))
    {
        EntityId entity_id = it.entity_id;
        CarBrainComponent* carbrain = get_component(engine,entity_id,COMP_CARBRAIN);
        if ((!has_component(engine, entity_id,COMP_MOVE))&&(!has_component(engine,entity_id,COMP_ACTION_ATTACH))&&(!has_component(engine,entity_id,COMP_ACTION_DROP)))
        {
            run_carbrain(carbrain);

            if (is_carbrain_decision_available(carbrain))
            {
                CarBrainDecision decision = pop_carbrain_decision(carbrain);
                execute_carbrain_decision(engine,decision,entity_id);
            }
        }
    }
    
}

void execute_carbrain_decision(Engine *engine, CarBrainDecision decision, EntityId carbrain_entity_id) {
    //only in part2! You can leave this empty in part1
    //TODO execute the decision
    //TODO: create component for every possible decision
    switch (decision)
    {
    case DECISION_FORWARD:{
        DirectionComponent* dir= get_component(engine,carbrain_entity_id,COMP_DIRECTION);
        MoveRequestComponent* move = create_component(engine,carbrain_entity_id,COMP_MOVE_REQUEST);
        move->direction = dir->direction;
        break;
    }
    case DECISION_TURN_LEFT: {
        DirectionComponent *dir = get_component(engine, carbrain_entity_id, COMP_DIRECTION);
        MoveRequestComponent *move = create_component(engine, carbrain_entity_id, COMP_MOVE_REQUEST);
        move->direction = LEFT_DIRECTION_2D(dir->direction);
        break;
    }
    case DECISION_TURN_RIGHT: {
        DirectionComponent *dir = get_component(engine, carbrain_entity_id, COMP_DIRECTION);
        MoveRequestComponent *move = create_component(engine, carbrain_entity_id, COMP_MOVE_REQUEST);
        move->direction = RIGHT_DIRECTION_2D(dir->direction);
        break;
    }
    case DECISION_IDLE:
        break;
    case DECISION_ATTACH: {
        create_component(engine, carbrain_entity_id, COMP_ACTION_ATTACH);
        break;
    }
    case DECISION_DROP: {
        create_component(engine, carbrain_entity_id, COMP_ACTION_DROP);
        break;
    }

    default:
        break;
    }

}

