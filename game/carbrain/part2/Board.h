#ifndef SYSPROG_BOARD_H
#define SYSPROG_BOARD_H

#include <fstream>
#include <vector>
#include <stdint.h>
#include "DecisionOutput.h"
#include "Assembler.h"

class Board;
#include "CPU.h"
#include "Bus.h"
#include "Mem.h"
#include "Sensors.h"
#include "DecisionOutput.h"

//Board bevat alle onderdelen: CPU, Bus, ...
class Board {
public:
    Board(spg_addr_t code_mem_size, spg_addr_t stack_mem_size);

    size_t loadAndStartCodeFromExeFile(const std::string& exeFilename);
    size_t loadAndStartCodeFromAsmFile(const std::string& asmFilename);
    size_t loadAndStartCodeFromExeBytes(const std::vector<uint8_t>& exe);
    size_t loadAndStartCodeFromAsmLines(const std::vector<std::string>& asmLines);

    [[nodiscard]] spg_addr_t getCodeMemStartAddress() const;
    [[nodiscard]] spg_addr_t getStackMemStartAddress() const;

    //run one CPU instruction
    void tick();
    //TODO add accessors as needed

    std::shared_ptr<Sensors> getSensorDataSink() const;
    std::shared_ptr<DecisionOutput> getDecisionSource() const;


#ifdef ONLY_IN_PART2_TESTS
        // allow tests to access parts of bus
        [[nodiscard]] std::shared_ptr<CPU> getCpu() const;
    [[nodiscard]] std::shared_ptr<Bus> getBus() const;
#endif
private:
    //TODO: voeg private velden toe (tip: gebruik shared_ptr)

    std::shared_ptr<CPU> cpu;
    std::shared_ptr<Bus> bus;
    std::shared_ptr<Mem> codeMem;
    std::shared_ptr<Mem> stackMem;
    std::shared_ptr<Sensors> sensors;
    std::shared_ptr<DecisionOutput> decisionOutput;
    spg_addr_t stack_mem_pointer;

};

#endif //SYSPROG_BOARD_H
