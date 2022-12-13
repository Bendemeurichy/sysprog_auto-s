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
    std::cout << "cpu reset to " << sp << " and " << code_start << std::endl;
}

void CPU::tick() {
    using namespace CPUInstruction;
    //TODO load 1 instruction
    //TODO increase %ip with bytesize of instruction
    //TODO execute 1 instruction
    //Hint:
    size_t bytes_read;
    std::cout<<"cpu-tick"<<std::endl;
    const Instruction instr = Instruction::decode([&]() -> uint8_t{ uint8_t operation = bus->read1(registers[REG_IP]++);return operation; },bytes_read);
    handleInstruction(instr);
}

void CPU::updateflag(bool flag) {
    if (flag) {
        this->flag = 0b1;
    } else {
        this->flag = 0;
    }
}

spg_register_t CPU::fetchOperand2(std::shared_ptr<CPUInstruction::Operand> operand) {
    using namespace CPUInstruction;
    if (operand->type == CPUInstruction::CPUInstructionOperandType::REGISTER) {

        std::shared_ptr<RegisterOperand> reg_operand = std::dynamic_pointer_cast<RegisterOperand>(operand);
        return registers[reg_operand->register_index];

    } else if (operand->type == CPUInstructionOperandType::MEM_REGISTER) {

        std::shared_ptr<MemRegisterOperand> mem_reg_operand = std::dynamic_pointer_cast<MemRegisterOperand>(operand);
        return ntohs(bus->read2_be(registers[mem_reg_operand->register_index] + mem_reg_operand->displacement));
    } else if (operand->type == CPUInstructionOperandType::MEM_IMMEDIATE) {
        
        std::shared_ptr<AddressOperand> mem_imm_operand = std::dynamic_pointer_cast<AddressOperand>(operand);
        return ntohs(bus->read2_be(mem_imm_operand->address));

    } else if (operand->type == CPUInstruction::CPUInstructionOperandType::IMMEDIATE) {
        
        std::shared_ptr<ImmediateOperand> immediate_operand = std::dynamic_pointer_cast<ImmediateOperand>(operand);
        return immediate_operand->immediate;

    } else {
        
        throw std::runtime_error("Unknown operand type");
    }
}

spg_register_t CPU::fetchOperand1(std::shared_ptr<CPUInstruction::Operand> operand) {
    using namespace CPUInstruction;
    if (operand->type == CPUInstruction::CPUInstructionOperandType::REGISTER) {

        std::shared_ptr<RegisterOperand> reg_operand = std::dynamic_pointer_cast<RegisterOperand>(operand);
        return registers[reg_operand->register_index];

    } else if (operand->type == CPUInstructionOperandType::MEM_REGISTER) {

        std::shared_ptr<MemRegisterOperand> mem_reg_operand = std::dynamic_pointer_cast<MemRegisterOperand>(operand);
        return bus->read1(registers[mem_reg_operand->register_index] + mem_reg_operand->displacement);
    } else if (operand->type == CPUInstructionOperandType::MEM_IMMEDIATE) {
        
        std::shared_ptr<AddressOperand> mem_imm_operand = std::dynamic_pointer_cast<AddressOperand>(operand);
        return bus->read1(mem_imm_operand->address);

    } else if (operand->type == CPUInstruction::CPUInstructionOperandType::IMMEDIATE) {
        
        std::shared_ptr<ImmediateOperand> immediate_operand = std::dynamic_pointer_cast<ImmediateOperand>(operand);
        return immediate_operand->immediate&0xFF;


    } else {
        
        throw std::runtime_error("Unknown operand type");
    }
}

void CPU::store1(CPUInstruction::Instruction instr, spg_register_t source) {
    //TODO bus->write1(target, source); check target type
    using namespace CPUInstruction;
    if (instr.target->type == CPUInstructionOperandType::REGISTER) {
        std::cout<<"store1 in register" << std::endl;
        std::shared_ptr<RegisterOperand> reg_operand = std::dynamic_pointer_cast<RegisterOperand>(instr.target);
        registers[reg_operand->register_index] = (source & 0xFF);

    } else if (instr.target->type == CPUInstructionOperandType::MEM_REGISTER) {
        std::cout<<"store1 in mem_register" << std::endl;
        std::shared_ptr<MemRegisterOperand> mem_reg_operand = std::dynamic_pointer_cast<MemRegisterOperand>(instr.target);
        bus->write1(registers[mem_reg_operand->register_index] + mem_reg_operand->displacement, htons(source));
    
    } else if (instr.target->type == CPUInstructionOperandType::MEM_IMMEDIATE) {
        std::cout<<"store1 in mem_immediate" << std::endl;
        std::shared_ptr<AddressOperand> mem_imm_operand = std::dynamic_pointer_cast<AddressOperand>(instr.target);
        bus->write1(mem_imm_operand->address, htons(source));
    
    } else if(instr.target->type == CPUInstructionOperandType::IMMEDIATE) {
        throw std::runtime_error("Can't store to immediate");
   
    }
    else {
        throw std::runtime_error("Unknown operand type");
    }
}

void CPU::store2(CPUInstruction::Instruction instr, spg_register_t source) {
    //TODO bus->write2(target, source); check target type
    using namespace CPUInstruction;
    if (instr.target->type == CPUInstructionOperandType::REGISTER) {
        std::cout << "store2 in register with value "<< std::to_string(source) << std::endl;
        std::shared_ptr<RegisterOperand> reg_operand = std::dynamic_pointer_cast<RegisterOperand>(instr.target);
        registers[reg_operand->register_index] = source;
    } else if (instr.target->type == CPUInstructionOperandType::MEM_REGISTER) {
        std::cout << "store2 in mem_register" << std::endl;
        std::shared_ptr<MemRegisterOperand> mem_reg_operand = std::dynamic_pointer_cast<MemRegisterOperand>(instr.target);
        bus->write2_be(registers[mem_reg_operand->register_index] + mem_reg_operand->displacement, source);
    } else if (instr.target->type == CPUInstructionOperandType::MEM_IMMEDIATE) {
        std::cout << "store2 in mem_immediate" << std::endl;
        std::shared_ptr<AddressOperand> mem_imm_operand = std::dynamic_pointer_cast<AddressOperand>(instr.target);
        bus->write2_be(mem_imm_operand->address, source);
    }
    else if (instr.target->type == CPUInstructionOperandType::IMMEDIATE)
    {
        throw std::runtime_error("Can't store to immediate");
    }
    else {
        throw std::runtime_error("Unknown operand type");
    }
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
            spg_register_t source = fetchOperand2(instr.source);
            store2(instr, source);
            updateflag(source == 0);
            std::cout << "MOV " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::MOVB:{
            spg_register_t source = fetchOperand1(instr.source);
            spg_register_t target = fetchOperand1(instr.target);
            store1(instr, source);
            updateflag(target==0);
            std::cout << "MOVB " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::PUSH:{
            spg_register_t source = fetchOperand2(instr.source);
            registers[REG_SP] -= 2;
            std::cout << "pushing to stack " << std::to_string(source) << std::endl;
            bus->write2_be(registers[REG_SP], source);
            break;
        }
        case Operation::POP:{
            spg_register_t target = fetchOperand2(instr.target);
            store2(instr, bus->read2_be(ntohs(registers[REG_SP])));
            registers[REG_SP] += 2;
            updateflag(bus->read2_be(registers[REG_SP])==0);
            std::cout << "popping from stack " << std::to_string(target) << std::endl;
            break;
        }
        case Operation::ADD :{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target + source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "ADD " << std::to_string(source) << std::endl;
            break;

        }
        case Operation::SUB:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target - source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "SUB " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::AND: {
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target & source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "AND " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::OR:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target | source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "OR " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::XOR:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target ^ source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "XOR " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::SHL:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target << source;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "SHL " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::INC:{
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target + 1;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "INC " << std::to_string(target) << std::endl;
            break;
        }
        case Operation::DEC:{
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target - 1;
            store2(instr, result);
            updateflag(result == 0);
            std::cout << "DEC " << std::to_string(target) << std::endl;
            break;
        }
        case Operation::CMP:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target - source;
            updateflag(result == 0);
            std::cout << "CMP " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::TEST:{
            spg_register_t source = fetchOperand2(instr.source);
            spg_register_t target = fetchOperand2(instr.target);
            spg_register_t result = target & source;
            updateflag(result == 0);
            std::cout << "TEST " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::JMP:{
            spg_register_t source = fetchOperand2(instr.source);
            registers[REG_IP] = source;
            std::cout << "JMP " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::JE:{
            spg_register_t source = fetchOperand2(instr.source);
            if (flag == 0){
                registers[REG_IP] = source;
            }
            std::cout << "JE " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::JNE:{
            spg_register_t source = fetchOperand2(instr.source);
            if (flag != 0){
                registers[REG_IP] = source;
            }
            std::cout << "JNE " << std::to_string(source) << std::endl;
            break;
        }
        case Operation::CALL:{
            spg_register_t source = fetchOperand2(instr.source);
            registers[REG_SP] -= 2;
            std::cout<<"calling "<<std::to_string(source)<<std::endl;
            bus->write2_be(registers[REG_SP],registers[REG_IP]);
            registers[REG_IP] = source;
            break;
        }
        case Operation::RET:{
            registers[REG_IP] = bus->read2_be(ntohs(registers[REG_SP]));
            registers[REG_SP] += 2;
            std::cout<<"returning"<<std::endl;
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
