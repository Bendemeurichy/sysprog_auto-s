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
    std::vector<uint8_t> data;
    std::ifstream file(exeFilename);
    if(!file.is_open()){
        throw BoardError("file" + exeFilename + "not found");
    }
    while (!file.eof())
    {
        uint8_t byte;
        file.read((char*)&byte, sizeof(byte));
        data.push_back(byte);
    }
    file.close();

    return loadAndStartCodeFromExeBytes(data);
}

size_t Board::loadAndStartCodeFromAsmFile(const std::string& asmFilename) {
    //Assemble de lijnen assembler code, en kopieer de resulterende binaire code het naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte
    //TODO implementeer
    std::vector<std::string> data;
    std::ifstream file(asmFilename);
    if(!file.is_open()){
        throw BoardError("file" + asmFilename + "not found");
    }
    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        data.push_back(line);
    }
    file.close();
    //std::cout << "file loaded" << std::endl;
    return loadAndStartCodeFromAsmLines(data);
}

size_t Board::loadAndStartCodeFromExeBytes(const std::vector<uint8_t>& exe) {
    //Kopieer binaire code naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte (= lengte van de "exe" vector)
    //TODO implementeer
    if(exe.size() > codeMem->manages(exe.size())){
        throw BoardError("code too big");
    }
    codeMem->setData(exe, exe.size());
    cpu->reset(getCodeMemStartAddress(),stackMem->getEnd()-2);
    return exe.size();
}

size_t Board::loadAndStartCodeFromAsmLines(const std::vector<std::string>& asmLines) {
    //Laadt het bestand met assembler code in, assemble het, en kopieer het naar het code geheugen van dit Board
    //Zet ook de registers correct zodat het programma zal starten.
    //Return de grootte van de binaire code, in byte
    //TODO implementeer
    std::vector<uint8_t> exe = assemble(asmLines,getCodeMemStartAddress());
    codeMem->setData(exe, exe.size());
    cpu->reset(getCodeMemStartAddress(),stackMem->getEnd()-2);
    return exe.size();
}

Board::Board(spg_addr_t code_mem_size, spg_addr_t stack_mem_size) {
    assert(code_mem_size > 0);
    assert(stack_mem_size > 0);
    codeMem = std::make_shared<Mem>(MEM_START, MEM_START + code_mem_size);
    stackMem = std::make_shared<Mem>(MEM_START + code_mem_size, MEM_START + code_mem_size + stack_mem_size);
    //std::cout << "codeMem start: " << codeMem->getStart() << std::endl;
    //std::cout << "stackMem end: " << stackMem->getEnd() << std::endl;
    
    bus = std::make_shared<Bus>();
    cpu = std::make_shared<CPU>(bus);
    bus->add_module(codeMem);
    bus->add_module(stackMem);
    sensors = std::make_shared<Sensors>();
    decisionOutput = std::make_shared<DecisionOutput>();
    bus->add_module(sensors);
    bus->add_module(decisionOutput);    

    //Implementeer: Voeg CPU, Bus, DecisionOutput, Sensors, en stack en code Memory toe aan het Board
}

void Board::tick() {
    //TODO run the CPU 1 tick
    //std::cout << "tick" << std::endl;
    cpu->tick();
}


#ifdef ONLY_IN_PART2_TESTS
std::shared_ptr<CPU> Board::getCpu() const {
    //TODO replace this by returning cpu stored in Board
    return cpu;
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
