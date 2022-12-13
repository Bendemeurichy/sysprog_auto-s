#include "Sensors.h"
#include "Exceptions.h"

using namespace std;

//TODO
Sensors::Sensors():Module(IO_VISION_START, IO_VISION_START +IO_VISION_SIZE){
    //TODO
}

uint8_t Sensors::read1(const spg_addr_t &address) {
    //TODO
    if (!manages(address)) {
        throw ModuleError("Sensors can't read from address " + std::to_string(address));
    }
    return data[address-start];
}

void Sensors::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    throw WriteNotAllowedError();
}

void Sensors::setSense(SensorReading* SensorReading){
    //TODO
    for (int i = 0; i < IO_VISION_SIZE-1; i++) {
        data[i] = SensorReading->bitmasks[i];
    }
    data[IO_VISION_SIZE-1] = SensorReading->bitmasks[IO_VISION_SIZE-1];
}
