#include "dragging_system.h"
#include <math.h>

void system_dragging_update(Engine* engine) {
    //TODO
    handle_attach(engine);
    handle_detach(engine);

}

void handle_attach(Engine* engine){
    EntityIterator it;
    search_entity_1(engine,COMP_ACTION_ATTACH,&it);
    while(next_entity(&it)){
        EntityId car = it.entity_id;
        DraggingComponent* dragging = get_component(engine,car,COMP_DRAGGING);
        LocationComponent* carloc = get_component(engine, car, COMP_LOCATION);
        EntityIterator crates;
        search_entity_by_location_1(engine,carloc->pos,0.3f,0,COMP_DRAGGABLE,&crates);

        EntityId crate = next_entity(&crates);
        
        DraggableComponent *drag = get_component(engine, crate, COMP_DRAGGABLE);
        if(!drag->dragged_by){
            if (!dragging->first_crate)
            {
                dragging->first_crate=crate;
                drag->dragged_by=car;                                                                                                                                                                         
            } else {
                EntityId prev_crate = dragging->first_crate;
                DraggableComponent* prev_drag = get_component(engine,prev_crate,COMP_DRAGGABLE);
                while(drag->next_crate!=0){
                    prev_crate=drag->next_crate;
                    prev_drag = get_component(engine,prev_crate,COMP_DRAGGABLE);
                }
                prev_drag->next_crate=crate;
                drag->dragged_by=prev_crate;
            }
        }
    }
}

void handle_detach(Engine* engine){

}

void handle_movement(Engine* engine){
    EntityIterator  it;
    search_entity_1(engine,COMP_DRAGGABLE,&it);
    while(next_entity(&it)){
        EntityId box=it.entity_id;
        LocationComponent* current_loc = get_component(engine,box,COMP_LOCATION);
        DraggableComponent* drag = get_component(engine,box,COMP_DRAGGABLE);
        LocationComponent* draggerloc = get_component(engine, drag->dragged_by, COMP_LOCATION);
        DirectionComponent* draggerdir = get_component(engine, drag->dragged_by, COMP_DIRECTION);
        if(sqrt(pow(draggerloc->pos[0]-current_loc->pos[0],2)-(pow(draggerloc->pos[1]-current_loc->pos[1],2)))>0.3f){
            current_loc->pos[0]=(draggerloc->pos[0]-0.3f)/2;
            current_loc->pos[1]=(draggerloc->pos[1]-0.3f)/2;
        }
    }
}
