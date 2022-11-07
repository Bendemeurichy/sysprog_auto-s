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
int main(int argc, char *argv[]) {
    // init random number generator
    srand(time(NULL));

    // SDL IMG init needed for graphics engine
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    // handle command line arguments
    if (strcmp(argv[1], "--help"))
    {
        printf("spg:help\n\nspg_part1 [ARG ...] [<level> ...]\n with ARG:\n--help :returns help of spg\n-- :next arguments are <level>"
               "\nAnd <level>:\n");
    }
    // TODO

    // inits the graphics system
    Renderer3D *renderer = (Renderer3D *)malloc(sizeof(Renderer3D));
    // renderer_3d_init(renderer, 640, 480);
    renderer_3d_init(renderer, 1280, 960);

    // initialises context, engine and assemblage, and adds systems
    Game *game = game_alloc(renderer);
    Engine *engine = &game->engine;

    // TODO: replace by loading only needed levels (depends on command line arguments)
    engine->level_count = TOTAL_LEVEL_COUNT;
    engine->levels = (Level *)calloc(engine->level_count, sizeof(Level));
    for (unsigned int i = 0; i < TOTAL_LEVEL_COUNT; i++)
    {
        levelloader_load_fixed_level(&engine->levels[i], i);
    }
    printf("Loaded %zu levels\n", game->engine.level_count);
    assert(TOTAL_LEVEL_COUNT == engine->level_count);

    // bootstrap the levelloader, time and world components
    create_base_entities(&game->engine);

    WorldComponent *world = (WorldComponent *)search_first_component(&game->engine, COMP_WORLD);
    world->game_request = GAME_REQUEST_INIT;
    world->game_state = GAME_PLAYING;

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
