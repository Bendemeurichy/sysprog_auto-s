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
    using namespace CPUInstruction;
    if (operand->type == CPUInstruction::CPUInstructionOperandType::REGISTER) {

        std::shared_ptr<RegisterOperand> reg_operand = std::dynamic_pointer_cast<RegisterOperand>(operand);
        return registers[reg_operand->register_index];

    } else if (operand->type == CPUInstructionOperandType::MEM_REGISTER) {

        std::shared_ptr<MemRegisterOperand> mem_reg_operand = std::dynamic_pointer_cast<MemRegisterOperand>(operand);
        return bus->read2_be(registers[mem_reg_operand->register_index] + mem_reg_operand->displacement);

    } else if (operand->type == CPUInstructionOperandType::MEM_IMMEDIATE) {
        
        std::shared_ptr<AddressOperand> mem_imm_operand = std::dynamic_pointer_cast<AddressOperand>(operand);
        return bus->read2_be(mem_imm_operand->address);

    } else if (operand->type == CPUInstruction::CPUInstructionOperandType::IMMEDIATE) {
        
        std::shared_ptr<ImmediateOperand> immediate_operand = std::dynamic_pointer_cast<ImmediateOperand>(operand);
        return immediate_operand->immediate;

    } else {
        
        throw std::runtime_error("Unknown operand type");
    }
}

void CPU::store1(CPUInstruction::Instruction instr, spg_register_t source) {
    //TODO
}

void CPU::store2(CPUInstruction::Instruction instr, spg_register_t source) {
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
            store2(instr, source);
            updateflag(source == 0);
            break;
        }
        case Operation::MOVB:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            store1(instr, source);
            updateflag(target==0);
            break;
        }
        case Operation::PUSH:{
            spg_register_t source = fetchOperand(instr.source);
            registers[REG_SP] -= 2;
            registers[REG_SP] = source;
            break;
        }
        case Operation::POP:{
            spg_register_t target = fetchOperand(instr.target);
            store2(instr, registers[REG_SP]);
            registers[REG_SP] += 2;
            updateflag(registers[REG_SP]==0);
            break;
        }
        case Operation::ADD :{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target + source;
            store2(instr, result);
            updateflag(result == 0);
            break;

        }
        case Operation::SUB:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target - source;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::AND: {
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target & source;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::OR:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target | source;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::XOR:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target ^ source;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::SHL:{
            spg_register_t source = fetchOperand(instr.source);
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target << source;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::INC:{
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target + 1;
            store2(instr, result);
            updateflag(result == 0);
            break;
        }
        case Operation::DEC:{
            spg_register_t target = fetchOperand(instr.target);
            spg_register_t result = target - 1;
            store2(instr, result);
            updateflag(result == 0);
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
