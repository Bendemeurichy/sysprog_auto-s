#ifndef SYSPROG_COMPONENTS_H
#define SYSPROG_COMPONENTS_H

#include "component_enums.h"

#include "game_util.h"
#include "../graphics/renderer_3d.h"
#include "../level/level.h"
#include "entity.h"

#include <stdbool.h>
#include <stdint.h>
#include <glmc.h>

#ifdef __cplusplus
extern "C++"
{
    class Board;
}
#endif

typedef enum GameRequest { GAME_REQUEST_NONE, GAME_REQUEST_EXIT, GAME_REQUEST_INIT, GAME_REQUEST_GOTO_LEVEL,
                           GAME_REQUEST_NEXT_LEVEL, GAME_REQUEST_PREV_LEVEL, GAME_REQUEST_RESTART_LEVEL,
                           GAME_REQUEST_SAVE, GAME_REQUEST_LOAD } GameRequest;

typedef enum GameState { GAME_EDIT, GAME_PLAYING, GAME_PAUSE, GAME_WIN, GAME_OVER } GameState;

/*
 * Note: Not every component needs a struct here, as some components do not store data.
 *       The presence or absence of these components is info in itself.
 */

typedef struct WorldComponent {
    int initial_level_nr;
    int requested_level_nr;
    GameRequest game_request;
    GameState game_state;
    bool show_debug;
} WorldComponent;

typedef struct LevelInfoComponent {
    uint8_t nr;
    char* name;
} LevelInfoComponent;

typedef struct EditorInfoComponent {
    uint8_t level_width;
    uint8_t level_height;
    const char* level_base_filename;
} EditorInfoComponent;

typedef struct TimeComponent {
    uint64_t time;
} TimeComponent;

typedef struct CameraComponent {
    t_vec3 current;
    t_vec3 target;
    uint64_t last_update;
} CameraComponent;

typedef struct MoveRequestComponent {
    Direction8 direction;
} MoveRequestComponent;

typedef struct LocationComponent {
    t_vec3 pos;
} LocationComponent;

typedef struct TileEditInfoComponent {
    t_ivec2 pos;
    Material ground_material;
    bool hasItem;
    LevelItem item;
} TileEditInfoComponent;

typedef struct GroundRectComponent {
    //relative to pos in LocationComponent
    t_vec3 pos0;
    t_vec3 pos1; // pos1.x > pos0.x, pos1.y > pos0.y, pos0.z = pos0.z
} GroundRectComponent;

typedef struct MoveComponent {
    uint64_t startTime;
    uint64_t duration;
    t_vec3 startPos;
    t_vec3 endPos;
} MoveComponent;

typedef struct CarArtComponent {
    ArtCarId art_id;
} CarArtComponent;

typedef struct GroundArtComponent {
    ArtGroundId art_id;
    int8_t offset_z;
} GroundArtComponent;

typedef struct BuildingArtComponent {
    ArtBuildingId art_id;
    int8_t offset_z;
} BuildingArtComponent;

typedef struct ColoredArtComponent {
    ArtColoredId art_id;
    ArtColor color;
} ColoredArtComponent;

typedef struct VariousArtComponent {
    ArtVariousId art_id;
} VariousArtComponent;

typedef struct InputReceiverComponent {
    int move_requested[DIRECTION_COUNT];
} InputReceiverComponent;

typedef struct DirectionComponent {
    Direction8 direction;
    bool found_right_wall;
} DirectionComponent;

typedef struct UpDownComponent {
    int8_t up_down;  //-1 = down   0 = none   1 = up
} UpDownComponent;

typedef struct ExitedComponent {
    uint64_t sleep;
    int8_t dir_tuned; // needed because empty structs are not supported on all compilers
} ExitedComponent;

typedef struct FilterComponent {
    int8_t required_crates;
} FilterComponent;

typedef struct MarkerComponent {
    //TODO
    ArtColor color;
} MarkerComponent;

typedef enum EditorTool {
    EDITOR_GRASS, EDITOR_ROAD, EDITOR_WATER, EDITOR_BUILDING, //0-3
    EDITOR_CAR, EDITOR_EXIT, EDITOR_CRATE, EDITOR_MARKER, EDITOR_DECORATION, EDITOR_FILTER, //4-9
    EDITOR_ROTATE, EDITOR_COLOR_OR_ART, EDITOR_DELETE, //10-12
    EDITOR__COUNT //=13
} EditorTool;

typedef struct EditorToolBarComponent {
    EditorTool selected_tool;
} EditorToolBarComponent;

typedef struct AutoMoveControlComponent {
    bool next; //1 step
    bool auto_next; //keep stepping
} AutoMoveControlComponent;

typedef struct CarBrainComponent {
#ifdef __cplusplus
    Board *board;
    // eventueel andere C++ velden
#else
    // C velden of placeholder
    uint8_t placeholder_replaceme;
#endif
      //needed because empty structs are not supported everywhere
} CarBrainComponent;

typedef struct CarBrainVisionComponent {
    //TODO
    uint8_t placeholder_replaceme;  //needed because empty structs are not supported everywhere
} CarBrainVisionComponent;


typedef struct DraggingComponent {
    //TODO
    EntityId first_crate;  //needed because empty structs are not supported everywhere
} DraggingComponent;

typedef struct DraggableComponent {
    EntityId dragged_by;
    EntityId next_crate;
} DraggableComponent;

#endif //SYSPROG_COMPONENTS_H
