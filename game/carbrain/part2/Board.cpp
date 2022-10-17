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

    //Implementeer: Voeg CPU, Bus, DecisionOutput, Sensors, en stack en code Memory toe aan het Board
}

void Board::tick() {
    //TODO run the CPU 1 tick
    //example implementation: cpu->tick();
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
    //example implementation: return code_mem->getStart();
    return 0;
}

spg_addr_t Board::getStackMemStartAddress() const {
    //TODO
    //example implementation: return stack_mem->getStart();
    return 0;
}

