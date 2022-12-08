#include "CPU.h"

#include "Bus.h"
#include "Exceptions.h"
#include "BitHelper.h"
#include <netinet/in.h>

CPU::CPU(std::shared_ptr<Bus>& bus) {
    this->bus = bus;
    

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

void CPU::updateflag(bool flag) {
    if (flag) {
        this->flag = 0b1;
    } else {
        this->flag = 0;
    }
}

spg_register_t CPU::fetchOperand(std::shared_ptr<CPUInstruction::Operand> operand) {
    //TODO
    return 0;
}

void CPU::store1(spg_register_t target, spg_register_t source) {
    //TODO
}

void CPU::store2(spg_register_t target, spg_register_t source) {
    //TODO
}

spg_register_t CPU::read2(spg_register_t target) {
    //TODO
    return 0;
}


// store1 stores 1 byte store2 2
//TODO: implement store1 and store2 and fetchoperand
void CPU::handleInstruction(const CPUInstruction::Instruction &instr)
{
    using namespace CPUInstruction;
    Operation op = instr.operation;
    switch (op){
        case Operation::NOP:{
            break;
            }
        case Operation::MOV:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            store2(target, source);
            updateflag(source == 0);
            break;
        }
        case Operation::MOVB:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            store1(target, source);
            updateflag(target==0);
            break;
        }
        case Operation::PUSH:{
            spg_register_t source = fetchOperand(instr.source);
            registers[REG_SP] -= 2;
            store2(registers[REG_SP], source);
            break;
        }
        case Operation::POP:{
            spg_register_t target = fetchOperand(instr.target);
            store2(target, read2(registers[REG_SP]));
            registers[REG_SP] += 2;
            updateflag(read2(registers[REG_SP])==0);
            break;
        }
    default:
        break;
    }
    
    // TODO

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
