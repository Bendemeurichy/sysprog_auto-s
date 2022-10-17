//forward declare
typedef struct Graphics Graphics;

#ifndef SYSPLEX_SDL_GRAPHICS_H
#define SYSPLEX_SDL_GRAPHICS_H

#include <SDL_video.h>
#include <SDL_image.h>

#include "spgsdl_texture.h"
#include "../es/game_util.h"


typedef enum TextureId {
    TEXTURE_TEST1,
    TEXTURE_TEST2,

    TEXTURE_LANDSCAPE,
    TEXTURE_CITY_EXTRA,
    TEXTURE_CITY,
    TEXTURE_BUILDINGS,
    TEXTURE_CARS,
    TEXTURE_PLATFORM,

    TEXTURE_EXTRA,
    TEXTURE_HUD,
    TEXTURE_BASE,
    TEXTURE_BASE_EDGE,

    TEXTURE__COUNT   //__ is not a type, but used to show this is a special value
} TextureId;

typedef struct Graphics {
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;

    uint32_t width;
    uint32_t height;

    t_vec4 background_color;

    Texture textures[TEXTURE__COUNT];
} Graphics;

void graphics_init(Graphics*, int width, int height);
void graphics_clear(Graphics*);
void graphics_free(Graphics*);

void graphics_begin_draw(Graphics*);
void graphics_end_draw(Graphics*);

void graphics_load_textures(Graphics* g);

#endif //SYSPLEX_SDL_GRAPHICS_H
