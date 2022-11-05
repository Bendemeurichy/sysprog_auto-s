#include "endlevel_system.h"

void handle_collision(Engine* engine);
void animate_ending(Engine* engine,ExitedComponent* exitanimation);

void system_endlevel_update(Engine* engine) {
    //TODO
    handle_collision(engine);

    EntityIterator it ; 
    search_entity_1(engine,COMP_EXIT_ACTIVATOR,&it);
    if(next_entity(&it)==0){
        ExitedComponent *exitanimation = search_first_component(engine, COMP_LEVEL_END_ANIMATION);
        
        animate_ending(engine, exitanimation);
        exitanimation->dir_tuned++;
        SDL_Delay(exitanimation->sleep);
        if(exitanimation->dir_tuned==5){
            WorldComponent* world = search_first_component(engine,COMP_WORLD);
            if (world->game_state == GAME_PLAYING || world->game_state == GAME_PAUSE || world->game_state == GAME_OVER)
            {
                world->game_request = GAME_REQUEST_NEXT_LEVEL;
            }
            if (world->game_state == GAME_WIN)
            {
                world->game_request = GAME_REQUEST_NONE;
            }
        }
    }
}

void animate_ending(Engine* engine,ExitedComponent* exitanimation){
        EntityIterator flags;
        search_entity_1(engine,COMP_EXIT,&flags);
        while (next_entity(&flags))
        {
            DirectionComponent* dir= get_component(engine,flags.entity_id,COMP_DIRECTION);
            dir->direction= direction_to_direction8(RIGHT_DIRECTION_2D(direction8_to_direction(dir->direction)));
        }
}

void handle_collision(Engine *engine){
    EntityIterator flags;
    search_entity_1(engine,COMP_EXIT,&flags);

    while(next_entity(&flags)){
        LocationComponent* exit_loc = get_component(engine,flags.entity_id,COMP_LOCATION);
        EntityIterator exiting_cars;
        search_entity_by_location_1(engine,exit_loc->pos,0.3f,1,COMP_EXIT_ACTIVATOR,&exiting_cars);
        
        if(next_entity(&exiting_cars)==1){
            EntityId car_id = exiting_cars.entity_id;
            if(has_component(engine,car_id,COMP_DRAGGING)){
                DraggingComponent* dragging= get_component(engine,car_id,COMP_DRAGGING);
                EntityId prev;
                EntityId next=dragging->first_crate;
                while(next!=NO_ENTITY){
                    prev=next;
                    DraggableComponent* draginfo=get_component(engine,prev,COMP_DRAGGABLE);
                    next=draginfo->next_crate;
                    delete_entity(engine,prev);
                }
                
            }
            delete_entity(engine, car_id);
        }
    }
}
