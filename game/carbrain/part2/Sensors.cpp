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
    if(address-start == SENSOR_POSITION_COUNT){
        return crates_dragged;
    } else {
        //std::cout<<"read1 sensors on address "<< std::to_string(address-start) << " returning " << std::to_string(data[address-start]) <<std::endl;
        return data[address-start];
    }
}

void Sensors::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    throw WriteNotAllowedError();
}

void Sensors::setSense(SensorReading* SensorReading){
    //TODO
    //std::cout<<"setSense" <<std::endl;
    for (int i = 0; i < SENSOR_POSITION_COUNT; i++) {
        data[i] = SensorReading->bitmasks[i];
        //std::cout<<"setSense sensors  "<< i << " to " << std::to_string(data[i]) <<std::endl;
    }
    //std::cout<< "crates dragged =" << std::to_string(SensorReading->crates_dragged) << std::endl;
    crates_dragged = SensorReading->crates_dragged;
}
