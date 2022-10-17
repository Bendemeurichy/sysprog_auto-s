#ifndef PUZZLEBOTS_CARBRAIN_IO_H
#define PUZZLEBOTS_CARBRAIN_IO_H

#include <stdint.h>
#include <stddef.h>

//////////////////////////////
//CarBrain Decision Output  //
//////////////////////////////

typedef enum CarBrainDecision {
    DECISION_FORWARD,
    DECISION_TURN_LEFT,
    DECISION_TURN_RIGHT,
    DECISION_IDLE,
    DECISION_ATTACH,
    DECISION_DROP,

    DECISION__COUNT
} CarBrainDecision;


//////////////////////////
//CarBrain Sensor Input //
//////////////////////////

//bitmask for CarBrain Sensor Input, fits in 1 byte
#define SENSE_ROAD 1u
#define SENSE_EXIT 2u
#define SENSE_MARKER_A 4u
#define SENSE_MARKER_B 8u
#define SENSE_MARKER_C 16u
#define SENSE_MARKER_D 32u
#define SENSE_FILTER 64u
#define SENSE_CRATE 128u

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

#define SENSOR_POSITION_COUNT 8

typedef struct SensorReading {
    uint8_t bitmasks[SENSOR_POSITION_COUNT];
    uint8_t crates_dragged;
} SensorReading;

int sensor_x_offset(size_t sensor_index);
int sensor_y_offset(size_t sensor_index);

#endif //PUZZLEBOTS_CARBRAIN_IO_H
