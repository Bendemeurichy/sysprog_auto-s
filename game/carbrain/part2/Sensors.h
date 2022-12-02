#ifndef SYSPROG_SENSORS_H
#define SYSPROG_SENSORS_H

#include <cassert>
#include <cstdint>
#include <stdint-gcc.h>
#include "Constants.h"
#include "Module.h"
#include "../carbrain_es_integration.h"
#include "BitHelper.h"

class SensorDataSink {
public:
    virtual ~SensorDataSink() = default;
    virtual void setSense(SensorReading* sensorReading) = 0;
};


class Sensors : public Module, public SensorDataSink {
    //TODO Implementeer de virtuele methoden van BotModule, om 1 of 2 bytes te lezen uit de SensorReading
    //TODO Implementeer de virtuele methoden van BotModule ivm schrijven door een exceptie te gooien
    //TODO Implementeer de virtuele methoden van SensorDataSink
};


#endif //SYSPROG_SENSORS_H
