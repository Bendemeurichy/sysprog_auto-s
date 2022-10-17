#include "sdl_graphics.h"

#include "spgsdl_texture.h"

Graphics* graphics_alloc(int width, int height) {
    Graphics* res = malloc(sizeof(Graphics));
    graphics_init(res, width, height);
    return res;
}

void graphics_clear(Graphics* g) {
    graphics_begin_draw(g);
    graphics_end_draw(g);
}

void graphics_free(Graphics* g) {
    for (int tex_id = 0; tex_id < TEXTURE__COUNT; tex_id++) {
        texture_free(&g->textures[tex_id]);
    }

    SDL_DestroyRenderer(g->sdl_renderer);
    g->sdl_renderer = NULL;

    SDL_DestroyWindow(g->sdl_window);
    g->sdl_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

void graphics_init(Graphics* graphics, int width, int height) {
    memset(graphics, 0, sizeof(Graphics));

    graphics->background_color[0] = 0;
    graphics->background_color[1] = 0;
    graphics->background_color[2] = 0.3f;
    graphics->background_color[2] = 1.0f;

    graphics->width = width;
    graphics->height = height;
    assert(width > 20);
    assert(height > 20);

    //Create window
    graphics->sdl_window = SDL_CreateWindow(
            "Simple Puzzle Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN );
    if(graphics->sdl_window == NULL) {
        fatal( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    }
    graphics->sdl_renderer = SDL_CreateRenderer(
            graphics->sdl_window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(graphics->sdl_renderer == NULL) {
        fatal("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }
    //Initialize renderer color
    SDL_SetRenderDrawColor(graphics->sdl_renderer,
                           graphics->background_color[0] * 0xFF,
                           graphics->background_color[1] * 0xFF,
                           graphics->background_color[2] * 0xFF,
                           graphics->background_color[3] * 0xFF);

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags)) {
        fatal("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }

    //SDL_ShowCursor(1);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    graphics_load_textures(graphics);
}

void graphics_begin_draw(Graphics* graphics) {
    SDL_SetRenderDrawColor(graphics->sdl_renderer,
                           graphics->background_color[0] * 0xFF,
                           graphics->background_color[1] * 0xFF,
                           graphics->background_color[2] * 0xFF,
                           graphics->background_color[3] * 0xFF);
    SDL_RenderClear(graphics->sdl_renderer);
}

void graphics_end_draw(Graphics* g) {
    SDL_RenderPresent(g->sdl_renderer);
}

void graphics_load_textures(Graphics* g) {
    char* texture_png_filenames[TEXTURE__COUNT] = {NULL};

    texture_png_filenames[TEXTURE_TEST1] = "textures/compass.png";
    texture_png_filenames[TEXTURE_TEST2] = "textures/compass2.png";

    texture_png_filenames[TEXTURE_CITY_EXTRA] = "textures/cityDetails_sheet.png";
    texture_png_filenames[TEXTURE_LANDSCAPE] = "textures/landscapeTiles_sheet.png";
    texture_png_filenames[TEXTURE_CITY] = "textures/cityTiles_sheet.png";
    texture_png_filenames[TEXTURE_BUILDINGS] = "textures/buildingTiles_sheet.png";
    texture_png_filenames[TEXTURE_CARS] = "textures/sheet_allCars.png";
    texture_png_filenames[TEXTURE_PLATFORM] = "textures/platform_sheet.png";
    texture_png_filenames[TEXTURE_EXTRA] = "textures/extra.png";
    texture_png_filenames[TEXTURE_HUD] = "textures/hud.png";
    texture_png_filenames[TEXTURE_BASE] = "textures/baseTiles_sheet.png";
    texture_png_filenames[TEXTURE_BASE_EDGE] = "textures/baseTilesEdged_sheet.png";

    for (int tex_id = 0; tex_id < TEXTURE__COUNT; tex_id++) {
        char* tex_image_filename = texture_png_filenames[tex_id];
        assert(tex_image_filename != NULL);

        texture_init(g, &g->textures[tex_id]);
        texture_load_png(&g->textures[tex_id], tex_image_filename);
        texture_set_blending(&g->textures[tex_id], 1);
        texture_set_alpha(&g->textures[tex_id], 255);
    }
}
