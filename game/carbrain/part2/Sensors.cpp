#include "Sensors.h"

using namespace std;

//TODO
Sensors::Sensors(spg_addr_t start, spg_addr_t stop) : Module(start, stop) {
    //TODO
}

uint8_t Sensors::read1(const spg_addr_t &address) {
    //TODO
}

void Sensors::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
}

spg_register_t Sensors::read2_be(const spg_addr_t &address) {
    //TODO
}

void Sensors::write2_be(const spg_addr_t &address, spg_register_t val) {
    //TODO
}

void Sensors::setSense(SensorReading* sensorReading) {
    //TODO
}

