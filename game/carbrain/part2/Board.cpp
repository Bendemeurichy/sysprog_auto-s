#include "Board.h"

#include <fstream>
#include <vector>
#include <stdint.h>
#include <filesystem>

using namespace std;

size_t Board::loadAndStartCodeFromExeFile(const std::string& exeFilename) {
    //Laadt het bestand met binaire code in, en kopieer het naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte
    //TODO implementeer
    return 0;
}

size_t Board::loadAndStartCodeFromAsmFile(const std::string& asmFilename) {
    //Assemble de lijnen assembler code, en kopieer de resulterende binaire code het naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte
    //TODO implementeer
    return 0;
}

size_t Board::loadAndStartCodeFromExeBytes(const std::vector<uint8_t>& exe) {
    //Kopieer binaire code naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte (= lengte van de "exe" vector)
    //TODO implementeer
    cpu.reset(code_Mem,stack_Mem);
    return 0;
}

size_t Board::loadAndStartCodeFromAsmLines(const std::vector<std::string>& asmLines) {
    //Laadt het bestand met assembler code in, assemble het, en kopieer het naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte
    //TODO implementeer
    return 0;
}

Board::Board(spg_addr_t code_mem_size, spg_addr_t stack_mem_size) {
    assert(code_mem_size > 0);
    assert(stack_mem_size > 0);
    code_Mem = make_shared<Mem>(Mem(MEM_START,code_mem_size));
    stack_Mem = make_shared<Mem>(Mem(MEM_START+code_mem_size,stack_mem_size));
    bus = make_shared<Bus>(Bus(code_Mem,stack_Mem));
    cpu = make_shared<CPU>(CPU(bus));
    //FIXME: fout
    decisionOutput = make_shared<DecisionOutput>(DecisionOutput(code_mem_size,MAX_MAX_DECISIONS));
    sensors = make_shared<Sensors>(Sensors(code_mem_size,stack_mem_size));

    //Implementeer: Voeg CPU, Bus, DecisionOutput, Sensors, en stack en code Memory toe aan het Board
}

void Board::tick() {
    //TODO run the CPU 1 tick
    cpu->tick();
}


#ifdef ONLY_IN_PART2_TESTS
std::shared_ptr<CPU> Board::getCpu() const {
    //TODO replace this by returning cpu stored in Board
    std::shared_ptr<CPU> dummyCpu(new CPU());
    return dummyCpu;
}

std::shared_ptr<Bus> Board::getBus() const {
    //TODO replace this by returning cpu stored in Board
    std::shared_ptr<Bus> dummyBus(new Bus());
    return dummyBus;
}
#endif

spg_addr_t Board::getCodeMemStartAddress() const {
    //TODO
    return code_Mem->getStart();
    
}

spg_addr_t Board::getStackMemStartAddress() const {
    //TODO
    return stack_Mem->getStart();
    
}

DecisionOutput* Board::getDecisionSource() {
    DecisionOutput* out = decisionOutput.get();
    return out;
}

Sensors* Board::getSensorDataSink() {
    Sensors* out = sensors.get();
    return out;
}
