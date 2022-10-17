#include "CPU.h"

#include "Bus.h"
#include "Exceptions.h"

#include <netinet/in.h>

CPU::CPU() {
    //TODO
}


void CPU::reset(spg_addr_t code_start, spg_addr_t sp) {
    //TODO set %ip and %sp to inital state
}


void CPU::tick() {
    //TODO load 1 instruction
    //TODO increase %ip with bytesize of instruction
    //TODO execute 1 instruction

    //Hint:
    //   size_t bytes_read;
    //   const Instruction instr = Instruction::decode(lambda_to_read_from_memory_using_bus, bytes_read);
}


#ifdef ONLY_IN_PART2_TESTS
spg_register_t CPU::getRegisterValue(size_t register_index) const {
    //TODO
    return 0;
}

void CPU::setRegisterValue(size_t register_index, spg_register_t value) {
    //TODO
}
#endif //ONLY_IN_PART2_TESTS
