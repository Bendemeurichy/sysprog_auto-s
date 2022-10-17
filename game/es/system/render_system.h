#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

typedef struct RenderStaticInfo RenderStaticInfo;

#include "../engine.h"
#include "../../graphics/renderer_3d.h"

//The render system needs some static info to be stored.
typedef struct RenderStaticInfo {
    Renderer3D* renderer;

    uint64_t last_fps_starttime;
    int current_fps_count;
    int last_fps_count;
} RenderStaticInfo;

void render_static_info_init(RenderStaticInfo*, Renderer3D*);
RenderStaticInfo* render_static_info_alloc(Renderer3D*);
void render_static_info_free(RenderStaticInfo*);

void system_render_update(Engine*);


typedef struct ScreenPosInfo {
    //input
    t_ivec2 screen_pos;  //position of which this has information

    //derived info
    t_vec2 iso_pos2;         //position on isometric grid
    t_vec3 tile_3d_pos;      //position of tile in isometric renderer
    Direction9 tile_dir9;    //which of the 9 sides of the tile
    uint8_t has_ground_rect;        //is there a tile at this position?
    t_vec3 tile_dir9_3d_pos; //position of current side of the tile, including height
    EntityId ground_rect_it; //entity with GroundRectComponent that is under mouse pointer (first if multiple)

    int8_t ground_offset_z;
} ScreenPosInfo;

ScreenPosInfo screen_pos_convert(Engine* engine, int screen_x, int screen_y);

void change_art(Engine*, EntityId, bool original);

#endif //RENDER_SYSTEM_H
