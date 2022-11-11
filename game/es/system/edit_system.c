// #include "move_system.h"
#include "edit_system.h"
#include "../assemblage.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>


void system_edit_update(Engine* engine)
    {
        // TODO process COMP_EDIT_REQUEST. (you will also need COMP_EDITOR_TOOLBAR).
        //      If an edit was made, set redo_art to true
        bool redo_art = false; // TODO set to true if any TileEditInfoComponent was changed

        EditorToolBarComponent *toolbar = search_first_component(engine, COMP_EDITOR_TOOLBAR);
        EntityId edit_request_id = search_first_entity_1(engine, COMP_EDIT_REQUEST);
        if (edit_request_id != NO_ENTITY)
        {
            TileEditInfoComponent *tile_info = get_component(engine, edit_request_id, COMP_TILE_EDIT_INFO);
            switch (toolbar->selected_tool)
            {
            case EDITOR_GRASS:
            {
                tile_info->ground_material = ART_GRASS;
                redo_art = true;
                break;
            }
            case EDITOR_ROAD:
            {
                tile_info->ground_material = ART_ROAD;
                redo_art = true;
                break;
            }
            case EDITOR_WATER:
            {
                tile_info->ground_material = ART_WATER;
                tile_info->hasItem = false;
                redo_art = true;
                break;
            }
            case EDITOR_BUILDING:
            {
                if (tile_info->ground_material == ART_GRASS && tile_info->pos[0] % 3 == 1 && tile_info->pos[1] % 3 == 1)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == BUILDING)
                        {
                            item->count = (item->count + 1) % 5;
                        }
                        else
                        {
                            LevelItem building;
                            building.type = BUILDING;
                            building.count = 1;
                            building.x = tile_info->pos[0];
                            building.y = tile_info->pos[1];
                            building.dir = N;

                            tile_info->item = building;
                        }
                    }
                    else
                    {
                        LevelItem building;
                        building.type = BUILDING;
                        building.count = 1;
                        building.x = tile_info->pos[0];
                        building.y = tile_info->pos[1];
                        building.dir = N;

                        tile_info->item = building;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_CAR:
            {
                srand((unsigned int) time(0));
                if (tile_info->ground_material == ART_ROAD)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == CAR)
                        {
                            item->color = (item->color + 1) % ART_COLOR__COUNT;
                        }
                        else
                        {
                            LevelItem car;
                            car.type = CAR;
                            car.color = rand()%ART_COLOR__COUNT;
                            car.x = tile_info->pos[0];
                            car.y = tile_info->pos[1];
                            car.dir = N;

                            tile_info->item = car;
                        }
                    }
                    else
                    {
                        LevelItem car;
                        car.type = CAR;
                        car.color = rand() % ART_COLOR__COUNT;
                        car.x = tile_info->pos[0];
                        car.y = tile_info->pos[1];
                        car.dir = N;

                        tile_info->item = car;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_EXIT:
            {
                if (tile_info->ground_material == ART_ROAD)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == EXIT)
                        {
                            item->dir = (item->dir + 1) % 4;
                        }
                        else
                        {
                            LevelItem flag;
                            flag.type = EXIT;
                            flag.x = tile_info->pos[0];
                            flag.y = tile_info->pos[1];
                            flag.dir = N;

                            tile_info->item = flag;
                        }
                    }
                    else
                    {
                        LevelItem flag;
                        flag.type = EXIT;
                        flag.x = tile_info->pos[0];
                        flag.y = tile_info->pos[1];
                        flag.dir = N;

                        tile_info->item = flag;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_CRATE: {
                srand((unsigned int)time(0));
                if (tile_info->ground_material == ART_ROAD)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == CRATE)
                        {
                            item->color = (item->color + 1) % 7;
                        }
                        else
                        {
                            LevelItem crate;
                            crate.type = CRATE;
                            crate.color = rand() % 7;
                            crate.x = tile_info->pos[0];
                            crate.y = tile_info->pos[1];
                            crate.dir = N;

                            tile_info->item = crate;
                        }
                    }
                    else
                    {
                        LevelItem crate;
                        crate.type = CRATE;
                        crate.color = rand() % 7;
                        crate.x = tile_info->pos[0];
                        crate.y = tile_info->pos[1];
                        crate.dir = N;

                        tile_info->item = crate;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            } 
            case EDITOR_MARKER:{
                srand((unsigned int)time(0));
                if (tile_info->ground_material == ART_ROAD)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == MARKER)
                        {
                            item->color = (item->color + 1) % 7;
                        }
                        else
                        {
                            LevelItem crate;
                            crate.type = MARKER;
                            crate.color = rand() % 7;
                            crate.x = tile_info->pos[0];
                            crate.y = tile_info->pos[1];
                            crate.dir = N;

                            tile_info->item = crate;
                        }
                    }
                    else
                    {
                        LevelItem crate;
                        crate.type = MARKER;
                        crate.color = rand() % 7;
                        crate.x = tile_info->pos[0];
                        crate.y = tile_info->pos[1];
                        crate.dir = N;

                        tile_info->item = crate;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break; 
            }
            case EDITOR_DECORATION: {
                srand((unsigned int)time(0));
                if (tile_info->ground_material == ART_GRASS)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == DECORATION)
                        {
                            item->art_variant = (item->art_variant + 1) % 5;
                        }
                        else
                        {
                            LevelItem rock;
                            rock.type = DECORATION;
                            rock.art_variant = rand() % 5;
                            rock.x = tile_info->pos[0];
                            rock.y = tile_info->pos[1];
                            rock.dir = N;

                            tile_info->item = rock;
                        }
                    }
                    else
                    {
                        LevelItem rock;
                        rock.type = DECORATION;
                        rock.art_variant = rand() % 5;
                        rock.x = tile_info->pos[0];
                        rock.y = tile_info->pos[1];
                        rock.dir = N;

                        tile_info->item = rock;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_FILTER: {
                if (tile_info->ground_material == ART_ROAD)
                {
                    if (tile_info->hasItem)
                    {
                        LevelItem *item = &tile_info->item;
                        if (item->type == FILTER)
                        {
                            item->count = (item->count + 1) % 5;
                        }
                        else
                        {
                            LevelItem filter;
                            filter.type = FILTER;
                            filter.count = 0;
                            filter.x = tile_info->pos[0];
                            filter.y = tile_info->pos[1];
                            filter.dir = N;

                            tile_info->item = filter;
                        }
                    }
                    else
                    {
                        LevelItem filter;
                        filter.type = FILTER;
                        filter.count = 0;
                        filter.x = tile_info->pos[0];
                        filter.y = tile_info->pos[1];
                        filter.dir = N;

                        tile_info->item = filter;
                        tile_info->hasItem = true;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_ROTATE:{
                if (tile_info->hasItem)
                {
                    LevelItem *item = &tile_info->item;
                    item->dir = LEFT_DIRECTION_2D(item->dir);
                    redo_art = true;
                }
                break;
            }
            case EDITOR_COLOR_OR_ART:{
                if (tile_info->hasItem)
                {
                    LevelItem *item = &tile_info->item;
                    if (item->type == CRATE ||  item->type == MARKER)
                    {
                        item->color = (item->color + 1) % 7;
                    }
                    else if (item->type == CAR)
                    {
                        item->color= (item->color + 1) % ART_COLOR__COUNT;
                    }
                    else if (item->type == DECORATION)
                    {
                        item->art_variant = (item->art_variant + 1) % 5;
                    }
                    redo_art = true;
                }
                break;
            }
            case EDITOR_DELETE:{
                if (tile_info->hasItem)
                {
                    tile_info->hasItem = false;
                    redo_art = true;
                }
                break;
            }
            default:
            {
                break;
            }
            }

            free_component_if_present(engine, edit_request_id, COMP_EDIT_REQUEST);
        }
        if (redo_art)
        {
            // an edit was made, so change reset art entities based on the updated TileEditInfoComponent
            Level level;
            leveledit_entities_to_level(engine, &level);
            clear_level(engine, true);
            set_level_entities(engine, &level, true, false, false);

            // don't forget to update the gridloc_cache whenever you've changed one or more gridposition components!
            update_gridloc_cache(&engine->es_memory);
        }
    }
