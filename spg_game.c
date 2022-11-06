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
    //init random number generator
    srand(time(NULL));
   
    // handle command line arguments
    if(argc>1){
        if(!strcmp(argv[1],"--help")){
            printf("spg:help\n\nSyntax: spg_part1 [ARG ...] [<level> ...]\n\n Where ARG:\n\t--help :returns help of spg\n\t-- :next arguments are <level>"
            "\n\nAnd <level>:\n\t-a valid level number\n\t-\"demo\": plays the demo level\n\t-a valid level filename\n\nit's possible to give multiple levels, only these levels will be played.");
            
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
    if(argc>1){ 
        //printf("more arguments parsed");
        if (!strcmp(argv[1],"--"))
        {
            if(argc==2){
                engine->level_count = TOTAL_LEVEL_COUNT;
                engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
                for (unsigned int i = 0; i < TOTAL_LEVEL_COUNT; i++)
                {
                    levelloader_load_fixed_level(&engine->levels[i], i);
                }
                //printf("Loaded %zu levels\n", game->engine.level_count);
                assert(TOTAL_LEVEL_COUNT == engine->level_count);
            } else {
                engine->level_count=argc-2; //argc - filename and arg "--"
                engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
                for(int i = 2;i<argc;i++){

                    if(!strcmp(tolowercase(argv[i]),"demo")){
                        //printf("demo level %s will be loaded", argv[i]);
                        levelloader_load_demo_level(&engine->levels[i-2]);

                    } else if(islevelnumber(argv[i])){
                        //printf("level with number %s will be loaded", argv[i]);
                        levelloader_load_fixed_level(&engine->levels[i-2],atoi(argv[i]));

                    } else {
                        //printf("level with name %s will be loaded",argv[i]);
                        levelloader_load_binary_level(&engine->levels[i-2],i-2,tolowercase(argv[i]));

                    }
                }
            }
        } else {
            engine->level_count = argc-1; // argc - filename
            engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
            for (int i = 1; i < argc; i++)
            {

                if (!strcmp(tolowercase(argv[i]), "demo"))
                {
                    //printf("demo level %s will be loaded", argv[i]);
                    levelloader_load_demo_level(&engine->levels[i-1]);
                }
                else if (islevelnumber(argv[i]))
                {   
                    printf("level with number %s will be loaded:arg %d",argv[i],i);
                    levelloader_load_fixed_level(&engine->levels[i-1], atoi(argv[i]));
                }
                else
                {
                    //printf("level with name %s will be loaded", argv[i]);
                    levelloader_load_binary_level(&engine->levels[i-2], i-2, tolowercase(argv[i]));
                }
            }
    }
    } else {
        //printf("no extra arguments given");
        engine->level_count = TOTAL_LEVEL_COUNT;
        engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
        for (unsigned int i = 0; i < TOTAL_LEVEL_COUNT; i++)
        {
            levelloader_load_fixed_level(&engine->levels[i], i);
        }
        printf("Loaded %zu levels\n", game->engine.level_count);
        assert(TOTAL_LEVEL_COUNT == engine->level_count);
    }

    //TODO: replace by loading only needed levels (depends on command line arguments)
    

    //bootstrap the levelloader, time and world components
    create_base_entities(&game->engine);

    WorldComponent* world = (WorldComponent*)search_first_component(&game->engine, COMP_WORLD);
    world->game_request = GAME_REQUEST_INIT;
    world->game_state = GAME_PLAYING;

    //Game loop
    while (1) {
        //Note: world entity and component might be changed by save/load system, so we need to look it up each time
        world = (WorldComponent*)search_first_component(&game->engine, COMP_WORLD);
        assert(world != NULL);
        if (world != NULL && world->game_request == GAME_REQUEST_EXIT) {
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

int islevelnumber(char* input){
    int length = strlen(input);
    for (int i = 0; i < length; i++){
        if (!isdigit(input[i]))
        {
            return 0;
        }
    }
    return 1;
}

char* tolowercase(char *str){
    
    for (int j = 0; str[j]; j++)
    {
        str[j] = tolower(str[j]);
    }
    return str;
}
