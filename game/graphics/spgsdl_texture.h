//forward declare
typedef struct Texture Texture;

#ifndef SYSPLEX_TEXTURE_H
#define SYSPLEX_TEXTURE_H

typedef struct Graphics Graphics;

#include <SDL_video.h>
#include <SDL_image.h>

#include <glmc.h>

typedef struct Texture {
    Graphics* graphics;

    SDL_Texture* texture;
    int width;
    int height;

    int blend_enabled;
} Texture;

#include "sdl_graphics.h"

void texture_init(Graphics* graphics, Texture* texture);
void texture_free(Texture* texture);

void texture_load_png(Texture* texture, const char* filename);
void texture_load_keyed_png(Texture* texture, const char* filename, t_vec3 key_color);
void texture_set_blending(Texture* texture, int blend);
void texture_set_alpha(Texture* texture, int alpha);

void texture_draw(Texture* texture, int screen_x, int screen_y, int offset_x, int offset_y, int w, int h);


#endif //SYSPLEX_TEXTURE_H
