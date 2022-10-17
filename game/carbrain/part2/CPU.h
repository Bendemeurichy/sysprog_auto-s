#ifndef SYSPROG_CPU_H
#define SYSPROG_CPU_H

#include <iostream>
#include <cstdint>
#include <memory>
#include <assert.h>

#include "Constants.h"
#include "CPUInstruction.h"

class CPU;
#include "Bus.h"


class CPU {
public:
    CPU();
    void reset(spg_addr_t code_start, spg_addr_t sp);
    void tick();

#ifdef ONLY_IN_PART2_TESTS
    //tests can access some inner working of the CPU, to test it more easily
    [[nodiscard]] spg_register_t getRegisterValue(size_t register_index) const;
    void setRegisterValue(size_t register_index, spg_register_t value);
#endif
private:
    //TODO Store CPU state (registers etc), shared pointer to Bus (to access memory and other modules), etc.
};


#endif //SYSPROG_CPU_H
