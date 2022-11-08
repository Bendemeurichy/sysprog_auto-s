// #include "move_system.h"
#include "edit_system.h"
#include "../assemblage.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>


void system_edit_update(Engine* engine) {
    //TODO process COMP_EDIT_REQUEST. (you will also need COMP_EDITOR_TOOLBAR). 
    //     If an edit was made, set redo_art to true
    bool redo_art = false; //TODO set to true if any TileEditInfoComponent was changed

    EditorToolBarComponent* toolbar =  search_first_component(engine,COMP_EDITOR_TOOLBAR);
    EntityId edit_request_id= search_first_entity_1(engine,COMP_EDIT_REQUEST);
    if(edit_request_id!=NO_ENTITY){
    TileEditInfoComponent* tile_info = get_component(engine,edit_request_id,COMP_TILE_EDIT_INFO);
    switch(toolbar->selected_tool){
        case EDITOR_GRASS: {
            tile_info->ground_material=ART_GRASS;
            redo_art=true;
            break;
        }
        case EDITOR_ROAD: {
            tile_info->ground_material=ART_ROAD;
            redo_art=true;
            break;
        }
        case EDITOR_WATER: {
            tile_info->ground_material=ART_WATER;
            redo_art=true;
            break;
        }
        case EDITOR_BUILDING: {
            if(tile_info->ground_material==ART_GRASS && tile_info->pos[0]%3==1 && tile_info->pos[1]%3==1){
                if(tile_info->hasItem){
                    LevelItem* item=&tile_info->item;
                    if(item->type==BUILDING){
                        item->count=(item->count+1)%5;
                    } else {
                        LevelItem building;
                        building.type = BUILDING;
                        building.count = 1;
                        building.x = tile_info->pos[0];
                        building.y = tile_info->pos[1];
                        building.dir = N;

                        tile_info->item = building;
                    }
                } else {
                LevelItem building;
                building.type=BUILDING;
                building.count=1;
                building.x=tile_info->pos[0];
                building.y = tile_info->pos[1];
                building.dir=N;

                tile_info->item=building;
                tile_info->hasItem=true;
                }
                redo_art=true;
            }
        }//3 done
        default: {
            break;
        }
    }
    free_component_if_present(engine, edit_request_id, COMP_EDIT_REQUEST);
    }
    if (redo_art) {
        //an edit was made, so change reset art entities based on the updated TileEditInfoComponent
        Level level;
        leveledit_entities_to_level(engine, &level);
        clear_level(engine, true);
        set_level_entities(engine, &level, true, false, false);

        //don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
        update_gridloc_cache(&engine->es_memory);
    }
    
}

