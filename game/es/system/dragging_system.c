#include "dragging_system.h"
#include <math.h>

void handle_attach(Engine* engine);
void handle_detach(Engine* engine);
void handle_movement(Engine* engine);

void system_dragging_update(Engine* engine) {
    //TODO
    handle_movement(engine);
    handle_attach(engine);
    handle_detach(engine);

}

void handle_attach(Engine* engine){
    EntityIterator it;
    search_entity_1(engine,COMP_ACTION_ATTACH,&it);
    while(next_entity(&it)){
        EntityId car = it.entity_id;
        DraggingComponent* dragging;
        if(!has_component(engine,car,COMP_DRAGGING)){
            dragging= create_component(engine,car,COMP_DRAGGING);
            dragging->first_crate=NO_ENTITY;
        }else {
            dragging=get_component(engine,car,COMP_DRAGGING);
        }
        LocationComponent* carloc = get_component(engine, car, COMP_LOCATION);

        EntityIterator crates;
        search_entity_by_location_1(engine,carloc->pos,0.3f,1,COMP_DRAGGABLE,&crates);
        if(next_entity(&crates)){
            EntityId crate = crates.entity_id;
            DraggableComponent* draginfo=get_component(engine,crate,COMP_DRAGGABLE);

            if(draginfo->dragged_by==NO_ENTITY){
                if(dragging->first_crate==NO_ENTITY){
                    dragging->first_crate=crate;
                    draginfo->dragged_by=car;
                } else {
                    DraggableComponent* prev_crate = get_component(engine,dragging->first_crate,COMP_DRAGGABLE);
                    EntityId crateid=dragging->first_crate;
                    while (prev_crate->next_crate != NO_ENTITY)
                    {
                        crateid = prev_crate->next_crate;
                        prev_crate = get_component(engine,prev_crate->next_crate,COMP_DRAGGABLE);
                        
                    }
                    prev_crate->next_crate=crate;
                    draginfo->dragged_by=crateid;
                }
            }
        }
        if(dragging->first_crate==NO_ENTITY){
            free_component(engine,car,COMP_DRAGGING);
        }
        free_component_if_present(engine,car,COMP_ACTION_ATTACH);
    }
}

void handle_detach(Engine* engine){
    EntityIterator it;
    search_entity_1(engine,COMP_ACTION_DROP,&it);
    while(next_entity(&it)){
        EntityId car=it.entity_id;
        DraggingComponent *dragging = get_component(engine, car, COMP_DRAGGING);
        if(has_component(engine,car,COMP_DRAGGING)){
            EntityId crate = dragging->first_crate;
            DraggableComponent* crate_info = get_component(engine,crate,COMP_DRAGGABLE);
            while(crate_info->next_crate!=NO_ENTITY){
                crate = crate_info->next_crate;
                crate_info = get_component(engine,crate,COMP_DRAGGABLE);
            }
            EntityId previd = crate_info->dragged_by;
            if(has_component(engine,previd,COMP_DRAGGABLE)){
                DraggableComponent* prev_crate= get_component(engine,previd,COMP_DRAGGABLE);
                prev_crate->next_crate = NO_ENTITY;
            } else {
                DraggingComponent* prev_crate= get_component(engine,previd,COMP_DRAGGING);
                prev_crate->first_crate = NO_ENTITY;
            }
            
            crate_info->dragged_by=NO_ENTITY;
        }
        if(dragging->first_crate==NO_ENTITY){
            free_component(engine,car,COMP_DRAGGING);
        }
        free_component_if_present(engine,car,COMP_ACTION_DROP);
    }
}

void handle_movement(Engine* engine){
    EntityIterator  it;
    search_entity_1(engine,COMP_DRAGGING,&it);

    while(next_entity(&it)){
        EntityId car=it.entity_id;
        DraggingComponent* cardrags = get_component(engine,car,COMP_DRAGGING);
        EntityId next= cardrags->first_crate;
        DraggableComponent *draginfo = get_component(engine, next, COMP_DRAGGABLE);
        while(next!=NO_ENTITY){
        LocationComponent* boxloc= get_component(engine,next,COMP_LOCATION);
        draginfo = get_component(engine, next, COMP_DRAGGABLE);
        EntityId dragging = draginfo->dragged_by;
        LocationComponent* draggingloc = get_component(engine,dragging,COMP_LOCATION);
        
        float dist = sqrt(pow(draggingloc->pos[0] - boxloc->pos[0], 2) + (pow(draggingloc->pos[1] - boxloc->pos[1], 2)));
        if(dist>0.3f){
            boxloc->pos[0]+=(((draggingloc->pos[0]-boxloc->pos[0]))*((dist-0.3)/2));
            boxloc->pos[1]+=(((draggingloc->pos[1]-boxloc->pos[1]))*((dist-0.3)/2));
        }
        
        next=draginfo->next_crate;
    }
}
}
