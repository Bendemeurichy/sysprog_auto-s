#include "move_system.h"

void handle_move(Engine* engine);
void handle_requests(Engine* engine);

void system_move_update(Engine *engine)
{
    // TODO
    handle_requests(engine);
    handle_move(engine);
    // don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
    update_gridloc_cache(&engine->es_memory);
}

void handle_move(Engine* engine){
    EntityIterator it;
    search_entity_1(engine,COMP_MOVE,&it);
    while (next_entity(&it))
    {
        EntityId entity_id = it.entity_id;
        uint64_t now = ((TimeComponent *)search_first_component(engine, COMP_TIME))->time;
        MoveComponent* move = get_component(engine,entity_id,COMP_MOVE);
        LocationComponent* current_loc = get_component(engine,entity_id,COMP_LOCATION);
        current_loc->pos[0] = move->startPos[0] + ((move->endPos[0] - move->startPos[0]) * (now - move->startTime) / move->duration);
        current_loc->pos[1] = move->startPos[1] + ((move->endPos[1] - move->startPos[1]) * (now - move->startTime) / move->duration);
        if (now >= (move->startTime  + move->duration))
        {
            free_component(engine,entity_id,COMP_MOVE);
        }
        update_gridloc_cache(&engine->es_memory);
    }
    
}

void handle_requests(Engine* engine){
    EntityIterator it;
    search_entity_1(engine, COMP_MOVE_REQUEST, &it);
    while (next_entity(&it)){
        EntityId entity_id = it.entity_id;
        LocationComponent *current_loc = get_component(engine, entity_id, COMP_LOCATION);
        MoveRequestComponent *moverequest = get_component(engine, entity_id, COMP_MOVE_REQUEST);

        t_vec3 goal = {RELATIVE8F_X(moverequest->direction, current_loc->pos[0]),
                    RELATIVE8F_Y(moverequest->direction, current_loc->pos[1]),
                    current_loc->pos[2]};
        

        if (!is_destination_blocked(engine,entity_id,goal)){
            // properly check if direction is possible
            if (!has_component(engine, entity_id, COMP_MOVE)){
                MoveComponent *current_move = create_component(engine, entity_id, COMP_MOVE);
                current_move->startTime = ((TimeComponent *)search_first_component(engine, COMP_TIME))->time;
                current_move->startPos[0] = current_loc->pos[0];
                current_move->startPos[1] = current_loc->pos[1];
                current_move->startPos[2] = current_loc->pos[2];
                current_move->endPos[0] = goal[0];
                current_move->endPos[1] = goal[1];
                current_move->endPos[2] = goal[2];
                current_move->duration = PLAYER_MOVE_MS;
                free_component(engine, entity_id, COMP_MOVE_REQUEST);
            }
        }
    }
}

bool is_destination_blocked(Engine* engine,EntityId moving_entity_id,t_vec3 dest){
    EntityIterator roads;
    search_entity_by_location_1(engine, dest, 0.1f, 1, COMP_ROAD, &roads);
    if(next_entity(&roads) == 1){
        return false;
    }
    if()
    return true;
}
