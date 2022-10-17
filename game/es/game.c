#include "game.h"

#include <stdlib.h>

void game_init(Game* game, Renderer3D* renderer) {
    game->renderer = renderer;
    engine_init(&game->engine, renderer);
}

Game* game_alloc(Renderer3D* renderer) {
    Game* res = malloc(sizeof(Game));
    game_init(res, renderer);
    return res;
}

void game_free(Game* game) {
    engine_free(&game->engine);
    //don't free graphics in Game
}
