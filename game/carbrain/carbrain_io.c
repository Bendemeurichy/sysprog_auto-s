#include <assert.h>
#include "carbrain_io.h"

// 8 positions around car (including own pos):
//
//   .
//  ...
//  .X.
//   .
//
//   0
//  123
//  456
//   7

int sensor_x_offset(size_t sensor_index) {
    assert(sensor_index < SENSOR_POSITION_COUNT);
    int offsets[] = {0, -1, 0, 1, -1, 0, 1, 0};
    return offsets[sensor_index];
}

int sensor_y_offset(size_t sensor_index) {
    assert(sensor_index < SENSOR_POSITION_COUNT);
    int offsets[] = {2, 1, 1, 1, 0, 0, 0, -1};
    return offsets[sensor_index];
}
