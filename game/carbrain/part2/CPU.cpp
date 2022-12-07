#include "CPU.h"

#include "Bus.h"
#include "Exceptions.h"

#include <netinet/in.h>

CPU::CPU(std::shared_ptr<Bus>& bus) {
    //TODO
    this->bus = bus;
    registers = std::array<spg_register_t, 8>();
}


void CPU::reset(spg_addr_t code_start, spg_addr_t sp) {
    //TODO set %ip and %sp to inital state
    registers[REG_IP] = code_start;
    registers[REG_SP] = sp;
}

void CPU::tick() {
    using namespace CPUInstruction;
    //TODO load 1 instruction
    //TODO increase %ip with bytesize of instruction
    //TODO execute 1 instruction

    //Hint:
    size_t bytes_read;
    const Instruction instr = Instruction::decode([&]() -> uint8_t{ uint8_t operation = bus->read1(registers[REG_IP]);return operation; },bytes_read);
    registers[REG_IP] += bytes_read;
    handleInstruction(instr);
}

void CPU::handleInstruction(const CPUInstruction::Instruction &instr)
{
    using namespace CPUInstruction;
    Operation op = instr.operation;
    switch (op){
        case Operation::NOP:
            break;
        case Operation::MOV:
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            if (instr.target->type == CPUInstructionOperandType::REGISTER)
            {
                store1(instr,source);
            }
            else
            {
                store2(instr,source);
            }
            updateflag()
            break;


    default:
        break;
    }
    
    // TODO
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
