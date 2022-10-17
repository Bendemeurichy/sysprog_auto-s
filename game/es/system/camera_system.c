#include "camera_system.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

static float move_closer(float cur, float target, float diff_seconds) {
    if (cur == target) return cur;
    float diff = cur - target;
    float partial = diff * (diff_seconds * (DRAG_PERCENT_SPEED_PER_S / 100.0f));
    if (fabsf(partial) < DRAG_MIN_SPEED_PER_TICK)
        partial = diff < 0.0f ? MAX(diff, -DRAG_MIN_SPEED_PER_TICK) : MIN(diff, DRAG_MIN_SPEED_PER_TICK);
    return cur - partial;
}

void system_camera_update(Engine* engine) {
    CameraComponent* camera = search_first_component(engine, COMP_CAMERA);

    if (camera == NULL) {
        //There is no camera
        return;
    }

    uint64_t last_update = camera->last_update;
    uint64_t now = ((TimeComponent*)search_first_component(engine, COMP_TIME))->time;
    camera->last_update = now;
    
    float diffSeconds = last_update == 0 ? 0.0f : (float)(now - last_update) / 1000.0f;
    
    if (!glmc_equal_vec3(camera->current, camera->target)) {
        camera->current[0] = move_closer(camera->current[0], camera->target[0], diffSeconds);
        camera->current[1] = move_closer(camera->current[1], camera->target[1], diffSeconds);
        camera->current[2] = move_closer(camera->current[2], camera->target[2], diffSeconds);
//        printf("Camera now centers at %f,%f,%f\n",
//               camera->current[0], camera->current[1], camera->current[2]);
    }
}
