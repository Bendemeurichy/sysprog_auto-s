#include "spgsdl_texture.h"

void texture_init(Graphics* graphics, Texture* texture) {
    texture->graphics = graphics;
    texture->texture = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->blend_enabled = 0;
}
void texture_free(Texture* texture) {
    assert(texture->texture != NULL);

    SDL_DestroyTexture(texture->texture);
    texture->texture = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->blend_enabled = 0;
}

void _texture_load_keyed_png(Texture* texture, const char* png_filename, int do_key, t_vec3 key_color) {
    assert(texture->texture == NULL);

    assert(texture->texture == NULL);
    SDL_Surface* surface = IMG_Load(png_filename);
    if(surface == NULL) {
        fatal("Unable to load image %s! SDL_image Error: %s\n", png_filename, IMG_GetError());
    }
    if (do_key) {
        SDL_SetColorKey(surface,SDL_TRUE, SDL_MapRGB(surface->format,key_color[0],key_color[1],key_color[2]));
    }
    texture->texture = SDL_CreateTextureFromSurface(texture->graphics->sdl_renderer, surface);
    texture->width = surface->w;
    texture->height = surface->h;
    SDL_FreeSurface(surface);
    if(texture->texture == NULL) {
        fatal("Unable to create texture from %s! SDL Error: %s\n", png_filename, SDL_GetError());
    }
}
void texture_load_png(Texture* texture, const char* png_filename) {
    assert(texture->texture == NULL);
    t_vec3 nokey = {0.0f, 0.0f, 0.0f};
    _texture_load_keyed_png(texture, png_filename, 0, nokey);
}
void texture_load_keyed_png(Texture* texture, const char* png_filename, t_vec3 key_color) {
    assert(texture->texture == NULL);
    _texture_load_keyed_png(texture, png_filename, 1, key_color);

}
void texture_set_blending(Texture* texture, int blend) {
    assert(texture->texture != NULL);
    if (texture->blend_enabled == blend) {
        return;
    }
    texture->blend_enabled = blend;
    if (blend) {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_BLEND);
    } else {
        SDL_SetTextureBlendMode(texture->texture, SDL_BLENDMODE_NONE);
    }
}
void texture_set_alpha(Texture* texture, int alpha) {
    assert(texture->texture != NULL);

    SDL_SetTextureAlphaMod(texture->texture, alpha);
}

void texture_draw(Texture* texture, int screen_x, int screen_y, int offset_x, int offset_y, int w, int h) {
    assert(texture->texture != NULL);

    SDL_Rect src_rect = {offset_x, offset_y, w, h };
    SDL_Rect dst_rect = {screen_x, screen_y, w, h };
    SDL_RenderCopy(texture->graphics->sdl_renderer, texture->texture, &src_rect, &dst_rect);
}
