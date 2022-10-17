#ifndef SYSPROG_CONSTANTS_H
#define SYSPROG_CONSTANTS_H

#include <cstdint>

// register and memory address types in the sysprog project.
// We'd use register_t and addr_t to keep the names simple, BUT register_t is used in <sys/types.h> already.
typedef uint16_t spg_register_t;
typedef uint16_t spg_addr_t;

//voorbeeld fixed memory layout:

//eerste 16 bytes niet gebruikt
#define USABLE_ADDRESS_START 0x0010u

//VisionInput van 0x0010 tot 0x0018
#define IO_VISION_START USABLE_ADDRESS_START
#define IO_VISION_SIZE 0x009u        //8 byte + 1 for dragged crate count

//DecisionOutput van 0x0018 tot 0x0023
#define IO_DECISION_START (IO_VISION_START + IO_VISION_SIZE)
#define IO_DECISION_SIZE  0x000Bu        //1 + max 10 decisions

//Code en stack memory van 0x0023 tot ...
#define MEM_START (USABLE_ADDRESS_START + IO_VISION_SIZE + IO_DECISION_SIZE)


#endif //SYSPROG_CONSTANTS_H
