#include "CPU.h"

#include "Bus.h"
#include "Exceptions.h"
#include "BitHelper.h"
#include <netinet/in.h>

    CPU::CPU(std::shared_ptr<Bus> bus)
{
    //TODO
    this->bus = bus;
    flag = std::make_shared<spg_register_t>();
    registers = {0,0,0,0,0,0,0,0};
}

void CPU::reset(spg_addr_t code_start, spg_addr_t sp) {
    //TODO
    registers[REG_IP] = code_start;
    registers[REG_SP] = sp;
}


void CPU::tick() {
    //TODO load 1 instruction
    //TODO increase %ip with bytesize of instruction
    //TODO execute 1 instruction
    //Hint:
    size_t bytes_read;
    const CPUInstruction::Instruction instr = CPUInstruction::Instruction::decode([&]() -> uint8_t { uint8_t operation = bus->read1(registers[REG_IP]);return operation; },bytes_read);\
    registers[REG_IP] += bytes_read;
    executeInstruction(instr);
}

void executeInstruction(CPUInstruction::Instruction instruction){
    //TODO
    CPUInstruction::Operation operation = instruction.operation;
    std::shared_ptr<CPUInstruction::Operand> source = instruction.source;
    std::shared_ptr<CPUInstruction::Operand> destination = instruction.target;
}

#ifdef ONLY_IN_PART2_TESTS
spg_register_t CPU::getRegisterValue(size_t register_index) const {
    return registers[register_index];
}

void CPU::setRegisterValue(size_t register_index, spg_register_t value) {
    //TODO
    registers[register_index] = value;
}
#endif //ONLY_IN_PART2_TESTS
