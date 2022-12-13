#include "input_system.h"

#include <SDL.h>

#include <string.h>
#include <assert.h>

void handle_game_exit(Engine* engine) {
    //signal the game loop in the main method to stop
    WorldComponent *world = search_first_component(engine, COMP_WORLD);
    world->game_request = GAME_REQUEST_EXIT;
}

static void handleKeyDown(Engine* engine, SDL_Keysym *keysym, InputReceiverComponent* input_recv_comp, EntityId input_recv_entity_id)
{
    switch( keysym->sym ) {
        case SDLK_w:
        case SDLK_z:
        case SDLK_KP_8:
        case SDLK_UP: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[N] = 1;
            }
            break;
        }
        case SDLK_s:
        case SDLK_KP_2:
        case SDLK_DOWN: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[S] = 1;
            }
            break;
        }
        case SDLK_a:
        case SDLK_q:
        case SDLK_KP_4:
        case SDLK_LEFT: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[W] = 1;
            }
            break;
        }
        case SDLK_d:
        case SDLK_KP_6:
        case SDLK_RIGHT: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[E] = 1;
            }
            break;
        }
        default:
            break;
    }
}

static void handleKeyUp(Engine* engine, SDL_Keysym *keysym, InputReceiverComponent* input_recv_comp, EntityId input_recv_entity_id)
{
    WorldComponent* world = search_first_component(engine, COMP_WORLD);
    switch( keysym->sym ) {
        case SDLK_F10: {
            handle_game_exit(engine);
            break;
        }
        case SDLK_F5: {
            world->game_request = GAME_REQUEST_SAVE;
            break;
        }
        case SDLK_F9: {
            world->game_request = GAME_REQUEST_LOAD;
            break;
        }
        case SDLK_ESCAPE: {
            handle_game_exit(engine);
            break;
        }
        case SDLK_KP_ENTER:
        case SDLK_RETURN2:
        case SDLK_RETURN:
        case SDLK_SPACE: {
            if (input_recv_comp != NULL) {
                create_component_if_not_present(engine, input_recv_entity_id, COMP_ACTION_ATTACH);
            }
            break;
        }
        case SDLK_BACKSPACE: {
            if (input_recv_comp != NULL) {
                create_component_if_not_present(engine, input_recv_entity_id, COMP_ACTION_DROP );
            }
            break;
        }
        case SDLK_n: { //"next"
            if (world->game_state == GAME_PLAYING || world->game_state == GAME_PAUSE || world->game_state == GAME_OVER) {
                world->game_request = GAME_REQUEST_NEXT_LEVEL;
            }
            if (world->game_state == GAME_WIN) {
                world->game_request = GAME_REQUEST_NONE;
            }
            break;
        }
        case SDLK_b: { //"back"
            if (world->game_state == GAME_PLAYING || world->game_state == GAME_PAUSE || world->game_state == GAME_OVER) {
                world->game_request = GAME_REQUEST_PREV_LEVEL;
            }
            if (world->game_state == GAME_WIN) {
                world->game_request = GAME_REQUEST_INIT;
            }
            break;
        }
        case SDLK_r: { //"reload"
            world->game_request = GAME_REQUEST_RESTART_LEVEL;
            break;
        }
        case SDLK_w:
        case SDLK_z:
        case SDLK_KP_8:
        case SDLK_UP: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[N] = 0;
            }
            break;
        }
        case SDLK_s:
        case SDLK_KP_2:
        case SDLK_DOWN: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[S] = 0;
            }
            break;
        }
        case SDLK_a:
        case SDLK_q:
        case SDLK_KP_4:
        case SDLK_LEFT: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[W] = 0;
            }
            break;
        }
        case SDLK_d:
        case SDLK_KP_6:
        case SDLK_RIGHT: {
            if (input_recv_comp != NULL) {
                input_recv_comp->move_requested[E] = 0;
            }
            break;
        }
        case SDLK_g: {
            if (input_recv_comp != NULL) {
                printf("Debug: input receiver present\n");
            } else {
                printf("Debug: no input receiver\n");
            }
            world->show_debug = !world->show_debug;
            printf("Debug: showing debug: %s\n", world->show_debug ? "TRUE" : "FALSE");
            break;
        }
        case SDLK_p: { //"pause"
            if (world->game_state == GAME_PLAYING) {
                world->game_state = GAME_PAUSE;
            } else if (world->game_state == GAME_PAUSE) {
                world->game_state = GAME_PLAYING;
            }
            break;
        }
        case SDLK_0:
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9: {
            int pressed = keysym->sym - '0';
            if (keysym->mod & KMOD_SHIFT) {
                pressed += 10;
            }
            if (pressed < EDITOR__COUNT) {
                EditorToolBarComponent *editor = search_first_component(engine, COMP_EDITOR_TOOLBAR);
                if (editor != NULL) {
                    editor->selected_tool = pressed;
                }
            }
            break;
        }
        case SDLK_l: {
            AutoMoveControlComponent* amdc = search_first_component(engine, COMP_AUTOMOVE_CONTROL);
            if (amdc != NULL) {
                amdc->next = true;
                printf("AutoMove step\n");
            }
            break;
        }
        case SDLK_k: {
            AutoMoveControlComponent* amdc = search_first_component(engine, COMP_AUTOMOVE_CONTROL);
            if (amdc != NULL) {
                amdc->auto_next = !amdc->auto_next;
                if (amdc->auto_next) {
                    printf("AutoMove actived\n");
                } else {
                    printf("AutoMove deactivated\n");
                }
            }
            break;
        }
        default:
            break;
    }
}

void system_input_update(Engine* engine) {
    EntityId input_recv_entity_id = search_first_entity_1(engine, COMP_INPUTRECEIVER);
    InputReceiverComponent* input_recv_comp = input_recv_entity_id == NO_ENTITY ? NULL : get_component(engine, input_recv_entity_id, COMP_INPUTRECEIVER);

    SDL_Event event;
    memset(&event, 0, sizeof(SDL_Event));
    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
            case SDL_KEYDOWN:
                handleKeyDown(engine, &event.key.keysym, input_recv_comp, input_recv_entity_id);
                break;
            case SDL_KEYUP:
                handleKeyUp(engine, &event.key.keysym, input_recv_comp, input_recv_entity_id);
                break;
            case SDL_QUIT:
                handle_game_exit(engine);
                break;
            case SDL_MOUSEMOTION: {
                //ignored
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                SDL_MouseButtonEvent *mouseButtonEvent = (SDL_MouseButtonEvent *) &event;
                int leftButtonDown = mouseButtonEvent->button == SDL_BUTTON_LEFT;
                int rightButtonDown = mouseButtonEvent->button == SDL_BUTTON_RIGHT;

//                printf("mouseButtonEvent at %d,%d left=%d right=%d\n",
//                       mouseButtonEvent->x, mouseButtonEvent->y,
//                       leftButtonDown, rightButtonDown);
                ScreenPosInfo screen_pos_info = screen_pos_convert(engine, mouseButtonEvent->x, mouseButtonEvent->y);
//                printf("   Iso Pos: %f,%f\n", screen_pos_info.iso_pos2[0], screen_pos_info.iso_pos2[1]);
//                printf("   Tile 3D Pos: %f,%f,%f\n", screen_pos_info.tile_3d_pos[0], screen_pos_info.tile_3d_pos[1],
//                       screen_pos_info.tile_3d_pos[2]);

                if (screen_pos_info.ground_rect_it != NO_ENTITY) {
//                    printf("   ground_rect_it: %u\n", screen_pos_info.ground_rect_it);
                    TileEditInfoComponent* edit = get_component(engine, screen_pos_info.ground_rect_it, COMP_TILE_EDIT_INFO);
                    if (edit != NULL) {
                        create_component_if_not_present(engine, screen_pos_info.ground_rect_it, COMP_EDIT_REQUEST);
                    } else {
//                        printf("   no TileEditInfoComponent\n");
                    }

                    bool can_receive_input = has_component(engine, screen_pos_info.ground_rect_it, COMP_INPUTRECEIVER_OPTION);
                    if (can_receive_input && leftButtonDown) {
                        //Remove old inputreceiver
                        EntityIterator it;
                        search_entity_1(engine, COMP_INPUTRECEIVER, &it);
                        while(next_entity(&it)) {
                            EntityId entity_id = it.entity_id;
                            free_component(engine, entity_id, COMP_INPUTRECEIVER);
                        }

                        //Create new input receiver
                        create_component(engine, screen_pos_info.ground_rect_it, COMP_INPUTRECEIVER);
                    }
                    if (rightButtonDown) {
                        //center camera on selected point
                        CameraComponent* camera = search_first_component(engine, COMP_CAMERA);
                        if (camera != NULL) {
//                            printf("   centering camera on %f,%f,%f\n",
//                                   screen_pos_info.tile_3d_pos[0],
//                                   screen_pos_info.tile_3d_pos[1],
//                                   screen_pos_info.tile_3d_pos[2]);
                            glmc_assign_vec3(camera->target, screen_pos_info.tile_3d_pos);
                        }
                    }
                } else {
//                    printf("   ground_rect_it: NONE\n");
                }

                break;
            }
            case SDL_MOUSEWHEEL: {
                //ignored
                break;
            }
        }
    }

    if (input_recv_entity_id == NO_ENTITY) {
        return;
    }
    assert(input_recv_entity_id != NO_ENTITY);
    assert(input_recv_comp != NULL);

    int move_dir = ANY_DIRECTION_ARRAY_CONDITION(input_recv_comp->move_requested, != 0);
    if (move_dir) {
        //Add MoveRequestComponent if not already present
        MoveRequestComponent* moverequest = NULL;
        if (has_component(engine, input_recv_entity_id, COMP_MOVE_REQUEST)) {
            moverequest = get_component(engine, input_recv_entity_id, COMP_MOVE_REQUEST);
        } else {
            moverequest = create_component(engine, input_recv_entity_id, COMP_MOVE_REQUEST);
        }
        assert(moverequest != NULL);
        for (Direction i = DIRECTION_FIRST; i <= DIRECTION_LAST; i++)
        {
            if (input_recv_comp->move_requested[i] == 1) {
                moverequest->direction = direction_to_direction8(i);
                DirectionComponent* dc = get_component(engine, input_recv_entity_id, COMP_DIRECTION);
                assert(dc != NULL);
                dc->direction = direction_to_direction8(i);
            }
        }
    } else {
        //Remove MoveRequestComponent if already present
        if (has_component(engine, input_recv_entity_id, COMP_MOVE_REQUEST)) {
            free_component(engine, input_recv_entity_id, COMP_MOVE_REQUEST);
        }
    }
}
