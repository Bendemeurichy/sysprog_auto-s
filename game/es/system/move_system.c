#include "move_system.h"

void system_move_update(Engine* engine) {
    //TODO
    EntityIterator it;
    search_entity_1(engine,COMP_MOVE_REQUEST,&it);
    while (next_entity(&it))
    {   
        EntityId entity_id = it.entity_id;
        LocationComponent* current_loc = get_component(engine,entity_id,COMP_LOCATION);
        MoveRequestComponent* moverequest = get_component(engine, entity_id, COMP_MOVE_REQUEST);
        if(moverequest->direction){
            //properly check if direction is possible
            if(!has_component(engine,entity_id,COMP_MOVE)){
                MoveComponent* current_move = create_component(engine,entity_id,COMP_MOVE);
                current_move->startTime = ((TimeComponent *)search_first_component(engine, COMP_TIME))->time;
                current_move->startPos[0] = current_loc->pos[0];
                current_move->startPos[1] = current_loc->pos[1];
                current_move->startPos[2] = current_loc->pos[2];
                current_move->endPos[0] = RELATIVE8F_X(moverequest->direction,current_loc->pos[0]);
                current_move->endPos[1] = RELATIVE8F_Y(moverequest->direction,current_loc->pos[1]);
                current_move->endPos[2] = current_loc->pos[2];
                current_move->duration = PLAYER_MOVE_MS;
            } else {

            }
        }
    }
    
    //don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
    update_gridloc_cache(&engine->es_memory);
}
