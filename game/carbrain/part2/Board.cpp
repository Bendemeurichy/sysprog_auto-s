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
    cpu->reset(Board::getCodeMemStartAddress(),Board::getStackMemStartAddress());
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
    codeMem = std::make_shared<Mem>(MEM_START, MEM_START + code_mem_size);
    stackMem = std::make_shared<Mem>(MEM_START + code_mem_size, MEM_START + code_mem_size + stack_mem_size);
    bus = std::make_shared<Bus>();
    cpu = std::make_shared<CPU>(bus);

    spg_addr_t modulesStartMemory = MEM_START + code_mem_size + stack_mem_size;
    decisionOutput = std::make_shared<DecisionOutput>(modulesStartMemory,modulesStartMemory+ MAX_MAX_DECISIONS);
    //sensors = std::make_shared<Sensors>(modulesStartMemory + MAX_MAX_DECISIONS );
    

    //Implementeer: Voeg CPU, Bus, DecisionOutput, Sensors, en stack en code Memory toe aan het Board
}

void Board::tick() {
    //TODO run the CPU 1 tick
    cpu->tick();
}


#ifdef ONLY_IN_PART2_TESTS
std::shared_ptr<CPU> Board::getCpu() const {
    //TODO replace this by returning cpu stored in Board
    return cpu
}

std::shared_ptr<Bus> Board::getBus() const {
    return bus;
}
#endif
std::shared_ptr<DecisionOutput> Board::getDecisionSource() const {
    return decisionOutput;
}

std::shared_ptr<Sensors> Board::getSensorDataSink() const {
    return sensors;
}

spg_addr_t Board::getCodeMemStartAddress() const {
    //TODO
    return codeMem->getStart();
    //return 0;
}

spg_addr_t Board::getStackMemStartAddress() const {
    //TODO
    return stackMem->getStart();
    //return 0;
}
