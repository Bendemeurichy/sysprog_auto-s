#include "automove_c_system.h"

int wall_detected(Engine *engine, EntityId moving_entity_id);

    void system_automove_c_update(Engine *engine)
{
    AutoMoveControlComponent* automove= search_first_component(engine, COMP_AUTOMOVE_CONTROL);
    if(automove->auto_next || automove->next){
        EntityIterator cars;
        search_entity_1(engine,COMP_INPUTRECEIVER_OPTION, &cars);
        while (next_entity(&cars) )
        {
            EntityId car = cars.entity_id;
            MoveRequestComponent* move;
            
        }
        
        automove->next=false;
    }
}

//detects walls, returns 0 if no walls 1 if wall on the right, 2 if wall in the front and 3 if car is in front
int wall_detected(Engine *engine, EntityId moving_entity_id)
{
    LocationComponent* current_loc = get_component(engine, moving_entity_id, COMP_LOCATION);
    DirectionComponent* current_direction = get_component(engine, moving_entity_id, COMP_DIRECTION);

    t_vec3 front = {RELATIVE8F_X(current_direction->direction, current_loc->pos[0]),
                    RELATIVE8F_Y(current_direction->direction, current_loc->pos[1]),
                    current_loc->pos[2]};

    t_vec3 right = {RELATIVE8F_X(RIGHT_DIRECTION8_90_2D(current_direction->direction), current_loc->pos[0]),
                RELATIVE8F_Y(RIGHT_DIRECTION8_90_2D(current_direction->direction), current_loc->pos[1]),
                current_loc->pos[2]};
    // check wall in front
    EntityIterator frontroads;
    search_entity_by_location_1(engine, front, 0.3f,0, COMP_ROAD, &frontroads);
    if (!next_entity(&frontroads)){
        return 2;
    }
    else {
        //check car in front
        
    }
    // check wall on the right
    EntityIterator rightroads;
    search_entity_by_location_1(engine, right, 0.3f, 0, COMP_ROAD, &rightroads);
    if (!next_entity(&rightroads))
    {
        return 1;
    }

    return 0;
}
