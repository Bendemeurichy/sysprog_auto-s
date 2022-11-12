#include "automove_c_system.h"

int wall_detected(Engine *engine,t_vec3 goal);

    void system_automove_c_update(Engine *engine)
{
    AutoMoveControlComponent* automove= search_first_component(engine, COMP_AUTOMOVE_CONTROL);
    if(automove->auto_next || automove->next){
        EntityIterator cars;
        search_entity_1(engine,COMP_INPUTRECEIVER_OPTION, &cars);
        while (next_entity(&cars) )
        {
            EntityId car = cars.entity_id;
            if(!has_component(engine,car,COMP_MOVE_REQUEST)){
            DirectionComponent* current_direction = get_component(engine,car,COMP_DIRECTION);
            LocationComponent* current_loc = get_component(engine,car,COMP_LOCATION);
            t_vec3 front = {RELATIVE8F_X(current_direction->direction, current_loc->pos[0]),
                            RELATIVE8F_Y(current_direction->direction, current_loc->pos[1]),
                            current_loc->pos[2]};

            t_vec3 right = {RELATIVE8F_X(RIGHT_DIRECTION8_90_2D(current_direction->direction), current_loc->pos[0]),
                            RELATIVE8F_Y(RIGHT_DIRECTION8_90_2D(current_direction->direction), current_loc->pos[1]),
                            current_loc->pos[2]};

            t_vec3 frontright = {RELATIVE8F_X(RIGHT_DIRECTION8_90_2D(current_direction->direction), front[0]),
                            RELATIVE8F_Y(RIGHT_DIRECTION8_90_2D(current_direction->direction), front[1]),
                            current_loc->pos[2]};

            if (current_direction->found_right_wall){
                if(wall_detected(engine,front)>=1){
                    current_direction->direction=LEFT_DIRECTION8_90_2D(current_direction->direction);
            } else if(!wall_detected(engine,right) && !wall_detected(engine,frontright)){
                current_direction->direction=RIGHT_DIRECTION8_90_2D(current_direction->direction);
            }
            else {
                MoveRequestComponent* move= create_component(engine,car,COMP_MOVE_REQUEST);
                move->direction=current_direction->direction;
            }
            } else {
                if(wall_detected(engine,right)==1){
                    current_direction->found_right_wall=true;
                } else if(wall_detected(engine,front)){
                    current_direction->direction=LEFT_DIRECTION8_90_2D(current_direction->direction);
                }
                else {
                    MoveRequestComponent* move= create_component(engine,car,COMP_MOVE_REQUEST);
                    move->direction=current_direction->direction;
                }
            }
    }

}
automove->next = false;
}
}

//detects walls, 0 if no wall on goal, 1 if wall on goal, 2 if car on goal
int wall_detected(Engine *engine,t_vec3 goal)
{
    EntityIterator cars;
    search_entity_by_location_1(engine, goal, 0.2f, 0, COMP_INPUTRECEIVER_OPTION, &cars);
    
    EntityIterator roads;
    search_entity_by_location_1(engine,goal,0.2f,0,COMP_ROAD,&roads);
    if(!next_entity(&roads)){
        return 1;
    } else if(next_entity(&cars)){
        return 2;
    } else {
        return 0;
    }
}
