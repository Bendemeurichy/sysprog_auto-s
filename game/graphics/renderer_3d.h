#ifndef SYSPLEX_RENDERER_3D_H
#define SYSPLEX_RENDERER_3D_H

#include "sdl_graphics.h"
#include "renderer_isometric.h"

#include <glmc.h>

/*
 * This "game renderer" can be given 3D coordinates and art_ground to render
 *
 * Internally, it will translate to the isometric "game renderer" and use it to draw everything.
 */

typedef enum Material {
    ART_DIRT,
    ART_GRASS,
    ART_WATER,
    ART_PAVED,
    ART_ROAD,
    ART_SAND,
    ART_FOAM, //Foam caused by water falling in water when a river ends in sea/lake
    ART_EDGE, //Any kind of edge that you can't drive on. Can be grass next to a ramp, paved or grass  next to a building, etc.

    ART__COUNT
} Material;
const char* material_to_string(Material material);

typedef enum ArtVariousType {
    ART_SIGN,
    ART_ROCK,
    ART_TREE,
    ART_PLANTS,
    ART_FLAG,
    ART_HEART,
    ART_COIN,
    ART_BOX,  //non-colored versions of crates and chests
    ART_FILTER,
    ART_VARIOUS_OTHER,
    ART_TILE_HIGHLIGHT,
    ART_TILE_SUB_HIGHLIGHT,
    ART_TILE_SUB_HIGHLIGHT_45,
    ART_TILE_SUB_HIGHLIGHT_CENTER,
    ART_UNKNOWN,

    ART_VARIOUS__COUNT
} ArtVariousType;

typedef enum ArtBuildingType {
    ART_BUILDING_GROUND_FLOOR,
    ART_BUILDING_STORY,
    ART_BUILDING_ROOF,

    ART_BUILDING__COUNT
} ArtBuildingType;

typedef enum ArtColoredType {
    ART_BUTTON_SQUARE,
    ART_BUTTON_SQUARE_PUSHED,
    ART_BUTTON_ROUND,
    ART_BUTTON_ROUND_PUSHED,
    ART_CRATE,
    ART_KEY,
    ART_HIGHLIGHT,
    ART_PLATFORM_THIN,
    ART_PLATFORM_THICK,
    ART_CONNECTOR,
    ART_CONNECTOR_AND,
    ART_CONNECTOR_OR,

    ART_COLORED__COUNT
} ArtColoredType;

typedef enum ArtPosition {
    ART_POS_CENTER,
    ART_POS_N,
    ART_POS_NE,
    ART_POS_E,
    ART_POS_SE,
    ART_POS_S,
    ART_POS_SW,
    ART_POS_W,
    ART_POS_NW,

    ART_POS__COUNT
} ArtPosition;

typedef enum ArtColor {
    ART_COLOR_GREY,
    ART_COLOR_RED,
    ART_COLOR_YELLOW,
    ART_COLOR_BLUE,
    ART_COLOR_GREEN,
    ART_COLOR_MAGENTA,
    ART_COLOR_CYAN,
    ART_COLOR_RAINBOW,

    ART_COLOR__COUNT
} ArtColor;

typedef struct ArtGround3D {
    TileId tile_id; //the isometric tile corresponding to this ArtGround3D
    uint8_t alt_art_index; //if there are multiple versions of this ArtGround3D, they have a different value here
    Material materials[ART_POS__COUNT]; //which material is on each position?
    uint8_t height [ART_POS__COUNT]; //what height has each position (relative to tile diamond)?
    uint8_t min_ground_height; //what height has the ground (relative to tile diamond)?
} ArtGround3D;

typedef enum BuildingRoofType {
    BUILDING_ROOF_FLAT,
    BUILDING_ROOF_POINTY,
    BUILDING_ROOF_ROUNDED
} BuildingRoofType;
#define RAND_ROOF_TYPE (rand() % (BUILDING_ROOF_ROUNDED+1))
#define COUNT_ROOF_TYPE (BUILDING_ROOF_ROUNDED+1u)

typedef enum BuildingColor {
    BUILDING_COLOR_NONE,
    BUILDING_COLOR_WHITE,
    BUILDING_COLOR_YELLOW,
    BUILDING_COLOR_BLACK,
    BUILDING_COLOR_BROWN,
    BUILDING_COLOR_RED,
    BUILDING_COLOR_BLUE,
    BUILDING_COLOR_GREEN
} BuildingColor;
#define RAND_BUILDING_COLOR ((rand() % BUILDING_COLOR_GREEN)+1)
#define RAND_BUILDING_SHARED_COLOR ((rand() % 3 == 1) ? BUILDING_COLOR_WHITE : (rand() % 2 == 1) ? BUILDING_COLOR_YELLOW : BUILDING_COLOR_RED)
#define COUNT_BUILDING_COLOR (BUILDING_COLOR_GREEN+1u)

typedef enum BuildingWindow {
    BUILDING_WINDOW_NONE,
    BUILDING_WINDOW_ROUNDED,
    BUILDING_WINDOW_BIG,
    BUILDING_WINDOW_VERY_BIG,
    BUILDING_WINDOW_SQUARE,
} BuildingWindow;
#define RAND_BUILDING_WINDOW (rand() % (BUILDING_WINDOW_SQUARE+1))
#define COUNT_BUILDING_WINDOW (BUILDING_WINDOW_SQUARE+1)

typedef enum BuildingExtra {
    BUILDING_EXTRA_NONE,
    BUILDING_EXTRA_DOOR,
    BUILDING_EXTRA_WINDOW_DOOR,
    BUILDING_EXTRA_SIDE_DOOR,
    BUILDING_EXTRA_ROUNDED_DOOR,
    BUILDING_EXTRA_GOODS,
    BUILDING_EXTRA_HANG_ROUNDED,
    BUILDING_EXTRA_HANG_SQUARE,
    BUILDING_EXTRA_HANG_DOOR,
    BUILDING_EXTRA_EXTENSION
} BuildingExtra;
#define COUNT_BUILDING_EXTRA (BUILDING_EXTRA_EXTENSION+1u)

typedef struct ArtBuilding3D {
    TileId tile_id_by_dir[4]; //the isometric tile_id's by direction4 they are facing
    ArtBuildingType type;

    BuildingRoofType roof_type;
    BuildingColor wall_color;
    BuildingWindow window_type;
    BuildingExtra extra;
    BuildingColor extra_color;

    uint8_t alt_art_index; //if there are multiple versions of this ArtBuilding3D, they have a different value here
} ArtBuilding3D;

typedef struct ArtColored3D {
    TileId tile_id_by_color[ART_COLOR__COUNT]; //the isometric tile_id's
    ArtColoredType type;
    uint8_t alt_art_index; //if there are multiple versions of this ArtColored3D, they have a different value here
    int8_t z_priority;
} ArtColored3D;

//other objects
typedef struct ArtVarious3D {
    TileId tile_id_by_dir[4]; //the isometric tile_id's by direction4 they are facing  (can be all same id if only 1 art_ground for each dir)
    ArtVariousType type;
    uint8_t alt_art_index; //if there are multiple versions of this ArtVarious3D, they have a different value here
    int8_t z_priority;
} ArtVarious3D;

typedef struct ArtCar3D {
    uint8_t car_type;
    uint8_t color;
    TileId tile_id_by_dir[8];      //N, NE, E, SE, S, ...
    TileId tile_id_up_by_dir[4];   //N, E, S, W
    TileId tile_id_down_by_dir[4]; //N, E, S, W
} ArtCar3D;

typedef uint16_t ArtGroundId;
#define ART_GROUND_ID_NONE 0xFFFF
#define ART_GROUND_MAX 500
static_assert(ART_GROUND_MAX < ART_GROUND_ID_NONE, "ART_GROUND_MAX < ART_GROUND_ID_NONE");
static_assert(ART_GROUND_ID_NONE < (1<<(sizeof(ArtGroundId)*8)), "ART_GROUND_ID_NONE must fit type ArtGroundId");

typedef uint8_t ArtBuildingId;
#define ART_BUILDING_ID_NONE 0xFF
#define ART_BUILDING_MAX 50
static_assert(ART_BUILDING_MAX < ART_BUILDING_ID_NONE, "ART_BUILDING_MAX < ART_BUILDING_ID_NONE");
static_assert(ART_BUILDING_ID_NONE < (1<<(sizeof(ArtBuildingId)*8)), "ART_BUILDING_ID_NONE must fit type ArtBuildingId");

typedef uint8_t ArtVariousId;
#define ART_VARIOUS_ID_NONE 0xFF
#define ART_VARIOUS_MAX 30
static_assert(ART_VARIOUS_MAX < ART_VARIOUS_ID_NONE, "ART_VARIOUS_MAX < ART_VARIOUS_ID_NONE");
static_assert(ART_VARIOUS_ID_NONE < (1 << (sizeof(ArtVariousId) * 8)), "ART_VARIOUS_ID_NONE must fit type ArtVariousId");

typedef uint8_t ArtCarId;
#define ART_CAR_ID_NONE 0xFF
#define ART_CAR_MAX 30
static_assert(ART_CAR_MAX < ART_CAR_ID_NONE, "ART_CAR_MAX < ART_CAR_ID_NONE");
static_assert(ART_CAR_ID_NONE < (1<<(sizeof(ArtCarId)*8)), "ART_CAR_ID_NONE must fit type ArtCarId");

typedef uint8_t ArtColoredId;
#define ART_COLORED_ID_NONE 0xFF
#define ART_COLORED_MAX 30
static_assert(ART_COLORED_MAX < ART_COLORED_ID_NONE, "ART_COLORED_MAX < ART_COLORED_ID_NONE");
static_assert(ART_COLORED_ID_NONE < (1<<(sizeof(ArtColoredId)*8)), "ART_COLORED_ID_NONE must fit type ArtColoredId");

#define BUFFER_MIN_X (3*-5)
#define BUFFER_MIN_Y (3*-5)
#define BUFFER_MAX_X (3*55)
#define BUFFER_MAX_Y (3*55)
#define BUFFER_DIM_X (1 + (BUFFER_MAX_X - BUFFER_MIN_X))
#define BUFFER_DIM_Y (1 + (BUFFER_MAX_Y - BUFFER_MIN_Y))
#define BUFFER_MAX_OBJECTS 512
#define BUFFER_MAX_RECTS 0xFFFF
#define BUFFER_OBJECT_NONE 0xFF
typedef uint16_t BufferedObjectId;
static_assert(1 << (sizeof(BufferedObjectId)*8) > BUFFER_MAX_OBJECTS, "BUFFER_MAX_OBJECTS must fit in BufferedObjectId type");

typedef struct BufferedGround {
    ArtGroundId art_id;
    t_vec3 position;
} BufferedGround;

#define MAX_HUD 16
typedef struct BufferedHUD {
    TileId tile_id;
    t_ivec2 position;
} BufferedHUD;

typedef struct BufferedObject {
    t_vec3 position;
    int8_t z_priority;

    TileId isometric_tile_id;

    BufferedObjectId next_object;
} BufferedObject;

typedef struct BufferedRect {
    t_vec3 pos0;
    t_vec3 pos1;
    uint8_t r; uint8_t g; uint8_t b; uint8_t a;
} BufferedRect;

typedef struct Renderer3D {
    RendererIsometric renderer_isometric;

    ArtGround3D art_ground[ART_GROUND_MAX];
    ArtGroundId art_ground_count;

    ArtVarious3D art_various[ART_VARIOUS_MAX];
    ArtVariousId art_various_count;

    ArtColored3D art_colored[ART_COLORED_MAX];
    ArtColoredId art_colored_count;

    ArtBuilding3D art_building[ART_BUILDING_MAX];
    ArtBuildingId art_building_count;

    ArtCar3D art_cars[ART_CAR_MAX];
    ArtCarId art_car_count;

    ArtGroundId art_ground_id_unknown;
    ArtGroundId art_colored_id_highlight;
    ArtVariousId art_various_id_tile_sub_highlight;
    ArtVariousId art_various_id_tile_sub_highlight_45;
    ArtVariousId art_various_id_tile_sub_highlight_center;
    ArtVariousId art_various_id_tile_highlight;
    ArtVariousId art_various_id_unknown;

    TileId tile_id_hud_gameover;
    TileId tile_id_hud_gamewin;
    TileId tile_id_hud_number[10];
    TileId tile_id_hud_actionbar;
    TileId tile_id_hud_actionbar_highlight;

    ArtGroundId art_ground_id_base_3x3[ART__COUNT];
    ArtGroundId art_ground_id_base_1x1[ART__COUNT];

    BufferedHUD buffer_hud[MAX_HUD];
    size_t buffer_hud_count;

    BufferedGround buffer_ground_by_pos[BUFFER_DIM_X][BUFFER_DIM_Y];
    BufferedObjectId buffer_object_by_pos[BUFFER_DIM_X][BUFFER_DIM_Y];
    BufferedObject buffer_objects[BUFFER_MAX_OBJECTS];
    size_t buffer_objects_count;

    BufferedRect rects[BUFFER_MAX_RECTS];
    size_t rects_count;
} Renderer3D;

////////// RENDER INIT/FREE and DRAW/CLEAR
void renderer_3d_init(Renderer3D*, int width, int height);
void renderer_3d_clear(Renderer3D*);
void renderer_3d_free(Renderer3D*);
void renderer_3d_begin_draw(Renderer3D *renderer);
void renderer_3d_end_draw(Renderer3D*);

void renderer_3d_ground_draw(Renderer3D* renderer, ArtGroundId art_id, t_vec3 position);
void renderer_3d_building_draw(Renderer3D* renderer, ArtBuildingId art_id, Direction dir, t_vec3 position);
void renderer_3d_colored_draw(Renderer3D* renderer, ArtColoredId art_id, ArtColor color, t_vec3 position);
void renderer_3d_various_draw(Renderer3D* renderer, ArtVariousId art_id, Direction dir, t_vec3 position, int always_on_top);
void renderer_3d_car_draw(Renderer3D* renderer, ArtCarId art_car_id, t_vec3 position, Direction8 dir, int8_t up_down);
void renderer_3d_hud_draw(Renderer3D* renderer, TileId hud_tile_id, int x, int y);
void renderer_3d_rect_draw(Renderer3D* renderer, t_vec3 pos0, t_vec3 pos1, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void renderer_3d_set_camera(Renderer3D* renderer, t_vec3 center_position);

////////// LOOKING UP ART
ArtGroundId renderer_3d_ground_lookup(Renderer3D* renderer, ArtGround3D* art_3d);

ArtColoredId renderer_3d_colored_lookup(Renderer3D* renderer, ArtColoredType type, uint8_t art_alt_index);
ArtVariousId renderer_3d_various_lookup(Renderer3D* renderer, ArtVariousType type, uint8_t art_alt_index);
ArtBuildingId renderer_3d_building_lookup(Renderer3D* renderer, ArtBuildingType type,
                                          BuildingRoofType roof_type, BuildingColor wall_color,
                                          BuildingWindow window_type, BuildingExtra extra, BuildingColor extra_color,
                                          uint8_t art_alt_index);

////////// REGISTERING ART
ArtGroundId add_special_art_ground(Renderer3D* renderer, TextureId texture_id, TileId tile_offset_in_texture);
ArtGroundId add_art_ground(Renderer3D* renderer,
                           TextureId texture_id,
                           TileId tile_offset_in_texture, uint8_t alt_art_index,
                           Material *materials,
                           uint8_t *height);

#endif //SYSPLEX_RENDERER_3D_H
