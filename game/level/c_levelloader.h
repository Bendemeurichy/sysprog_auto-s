#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <glmc.h>
#include "../es/game_util.h"
#include "../graphics/renderer_3d.h"

#include "level.h"
#include "../es/engine.h"

void levelloader_load_binary_level(Level *level, unsigned int level_nr, const char *filename);
void levelloader_load_fixed_level(Level* level, unsigned int level_nr);
void levelloader_load_demo_level(Level* level);

#endif //LEVELLOADER_H
