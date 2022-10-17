#ifndef GAME_H
#define GAME_H

#include "engine.h"

#include "assemblage.h"
#include "../graphics/renderer_3d.h"

typedef struct Game {
    Renderer3D* renderer;
    Engine engine;
} Game;

void game_init(Game*, Renderer3D*);
Game* game_alloc(Renderer3D*);
void game_free(Game*);

#endif //GAME_H
