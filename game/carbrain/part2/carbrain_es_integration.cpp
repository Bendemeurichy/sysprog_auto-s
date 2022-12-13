#include "../carbrain_es_integration.h"

#include "Board.h"
#include "Exceptions.h"

#include <stdlib.h>
#include <stdint.h>
#include <netinet/in.h>

extern "C" {

bool is_carbrain_available() {
    return true;
}

void free_CarBrainComponent(CarBrainComponent* carBrainComponent) {
    assert(carBrainComponent != nullptr);
    //TODO geef de board pointer in carBrainComponent vrij
    delete carBrainComponent->board;
}

void init_CarBrainComponent(CarBrainComponent *carBrainComponent,
        unsigned int level_nr,
        unsigned int carbrain_index) {
    //TODO
    //Example solution:
       carBrainComponent->board = new Board(1024, 256);
       std::string asmFilename = "level"+std::to_string(level_nr)+".carbrain_asm";
       std::cout<<asmFilename<<std::endl; /* TODO use correct file depending on level_nr */
       spg_addr_t code_start_addr = carBrainComponent->board->getCodeMemStartAddress();
       carBrainComponent->board->loadAndStartCodeFromAsmFile(asmFilename);

    //if BitWriter is not yet implemented, you can also read the programs directly from the provided *.carbrain_exe files:
      //carBrainComponent->board = new Board(1024, 256);
      //std::string exeFilename = "level1.carbrain_exe"; /* TODO use correct file depending on level_nr */
      //spg_addr_t code_start_addr = carBrainComponent->board->getCodeMemStartAddress();
      //carBrainComponent->board->loadAndStartCodeFromExeFile(exeFilename); //This requires BitReader to be implemented

    //Note that this exe is compiled according to the fixed memory layout described in Constants.h
}

int is_carbrain_decision_available(CarBrainComponent *carBrainComponent) {
    assert(carBrainComponent != nullptr);
    //TODO
    return carBrainComponent->board->getDecisionSource()->isDecisionAvailable();
    //return false;

}

CarBrainDecision pop_carbrain_decision(CarBrainComponent* carBrainComponent) {
    assert(carBrainComponent != nullptr);
    //TODO
    return carBrainComponent->board->getDecisionSource()->popDecision();
    //return CarBrainDecision::DECISION_FORWARD;

}

void set_carbrain_sense(CarBrainComponent* carBrainComponent, SensorReading* sensor_reading) {
    //TODO
    //std::cout<<"set_carbrain_sense"<<std::endl;
    carBrainComponent->board->getSensorDataSink()->setSense(sensor_reading);
}

void run_carbrain(CarBrainComponent* carBrainComponent) {
    //TODO
    //example solution:
    assert(carBrainComponent != nullptr);
    assert(carBrainComponent->board != nullptr);
    

    //Run 50 steps, or until decision has been made
    for (int i = 0; i < 20; i++) {
    if (carBrainComponent->board->getDecisionSource()->isDecisionAvailable()){
        std::cout<<"decision available"<<std::endl;
        break;
    } else {
    carBrainComponent->board->tick();
    }
    }
}

} //extern C

