#include <stdio.h>
#include "game/graphics/renderer_isometric.h"
#include "game/es/game.h"
#include "game/graphics/renderer_3d.h"
#include "game/level/fixed_level_info.h"
#include "game/level/c_levelloader.h"

#include <SDL.h>
#include <SDL_timer.h>

#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

int islevelnumber(char *input);
char *tolowercase(char *str);

int main(int argc, char *argv[])
{
    // init random number generator
    srand(time(NULL));

    // handle command line arguments
    if (argc ==2)
    {
        if (!strcmp(argv[1], "--help"))
        {
            printf("spg:help\n\nSyntax: spg_part1_edit [ARG ...] [<level> ...]\n\n Where ARG is one of:\n\t--help :returns help of spg\n\t--: Treat all further arguments as literal <level> filenames"
                "\n\nAnd <level>:\n\t-a valid level nr: edit the level with that number (0 = demo level)\n\t  - \" demo \": play the demo level (any case allowed, ex: DEMO, deMo, ...)\n\t- a filename: play the level in the file\n\n.<level> must be specified exactly one");

            return EXIT_SUCCESS;
        }
    }

    // SDL IMG init needed for graphics engine
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    // inits the graphics system
    Renderer3D *renderer = (Renderer3D *)malloc(sizeof(Renderer3D));
    // renderer_3d_init(renderer, 640, 480);
    renderer_3d_init(renderer, 1280, 960);

    // initialises context, engine and assemblage, and adds systems
    Game *game = game_alloc(renderer);
    Engine *engine = &game->engine;
    if (argc == 3)
    {
        // printf("more arguments parsed");
        if (!strcmp(argv[1], "--"))
        {
            engine->level_count = 1; // argc - filename and arg "--"
            engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
            // printf("level with name %s will be loaded",argv[i]);
            levelloader_load_binary_level(&engine->levels[0], 0, tolowercase(argv[2]));
        } else {

            fatal("spg:help\n\nSyntax: spg_part1_edit [ARG ...] [<level> ...]\n\n Where ARG is one of:\n\t--help :returns help of spg\n\t--: Treat all further arguments as literal <level> filenames"
                "\n\nAnd <level>:\n\t-a valid level nr: edit the level with that number (0 = demo level)\n\t  - \" demo \": play the demo level (any case allowed, ex: DEMO, deMo, ...)\n\t- a filename: play the level in the file\n\n.<level> must be specified exactly one");
            game_free(game);
            free(game);

            renderer_3d_clear(renderer);
            renderer_3d_free(renderer);
            free(renderer);

            IMG_Quit();
            return EXIT_SUCCESS;   
        }
            
        }
        else if (argc==2)
        {
            engine->level_count = argc - 1; // argc - filename
            engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
            
                if ((!strcmp(argv[1], "0")) || (!strcmp(tolowercase(argv[1]), "demo")))
                {
                    // printf("demo level %s will be loaded", argv[i]);
                    levelloader_load_demo_level(&engine->levels[0]);
                    Level *clevel = &engine->levels[0];
                    clevel->nr = 0;
                }
                else if (islevelnumber(argv[1]))
                {
                    printf("level with number %s will be loaded:arg %d", argv[1], 0);
                    levelloader_load_fixed_level(&engine->levels[0], atoi(argv[1]) - 1);
                    Level *clevel = &engine->levels[0];
                    clevel->nr = 0;
                }
                else
                {
                    // printf("level with name %s will be loaded", argv[i]);
                    levelloader_load_binary_level(&engine->levels[0], 0, tolowercase(argv[1]));
                }
            
        }
    else
    {
        fatal("spg:help\n\nSyntax: spg_part1_edit [ARG ...] [<level> ...]\n\n Where ARG is one of:\n\t--help :returns help of spg\n\t--: Treat all further arguments as literal <level> filenames"
            "\n\nAnd <level>:\n\t-a valid level nr: edit the level with that number (0 = demo level)\n\t  - \" demo \": play the demo level (any case allowed, ex: DEMO, deMo, ...)\n\t- a filename: play the level in the file\n\n.<level> must be specified exactly one");
        game_free(game);
        free(game);

        renderer_3d_clear(renderer);
        renderer_3d_free(renderer);
        free(renderer);

        IMG_Quit();
        return EXIT_SUCCESS;
    }

    // TODO: replace by loading only needed levels (depends on command line arguments)

    // bootstrap the levelloader, time and world components
    create_base_entities(&game->engine);

    WorldComponent *world = (WorldComponent *)search_first_component(&game->engine, COMP_WORLD);
    world->game_request = GAME_REQUEST_INIT;
    world->game_state = GAME_EDIT;
    create_component(engine,get_new_entity_id(engine),COMP_EDITOR_INFO);
    create_component(engine,get_new_entity_id(engine),COMP_TILE_EDIT_INFO);
    create_component(engine,get_new_entity_id(engine),COMP_EDITOR_TOOLBAR);

    // Game loop
    while (1)
    {
        // Note: world entity and component might be changed by save/load system, so we need to look it up each time
        world = (WorldComponent *)search_first_component(&game->engine, COMP_WORLD);
        assert(world != NULL);
        if (world != NULL && world->game_request == GAME_REQUEST_EXIT)
        {
            break;
        }

        engine_update(&game->engine);
    }
    
    // cleanup
    game_free(game);
    free(game);
    
    renderer_3d_clear(renderer);
    renderer_3d_free(renderer);
    free(renderer);

    IMG_Quit();
    return EXIT_SUCCESS;
}

int islevelnumber(char *input)
{
    int length = strlen(input);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(input[i]))
        {
            return 0;
        }
    }
    return 1;
}

char *tolowercase(char *str)
{

    for (int j = 0; str[j]; j++)
    {
        str[j] = tolower(str[j]);
    }
    return str;
}
