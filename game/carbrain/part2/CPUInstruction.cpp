#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#pragma ide diagnostic ignored "modernize-return-braced-init-list"
#include <utility>

#include <cassert>
#include "CPUInstruction.h"
#include "Exceptions.h"

using namespace CPUInstruction;

Instruction::Instruction(const Operation& operation,
                         std::shared_ptr<Operand> source,
                         std::shared_ptr<Operand> target) :
        operation(operation), source(std::move(source)), target(std::move(target)) {
    
}
Instruction::Instruction(const Operation& operation,
                         std::shared_ptr<Operand> operand) :
        operation(operation), source(std::move(operand)), target(nullptr) {
    
}
Instruction::Instruction(const Operation& operation) :
        operation(operation), source(nullptr), target(nullptr) {
}



Operand::Operand(CPUInstructionOperandType type) : type(type) {

}

RegisterOperand::RegisterOperand(uint8_t register_index)
        : Operand(REGISTER), register_index(register_index) {
    
}

MemRegisterOperand::MemRegisterOperand(uint8_t register_index, int8_t displacement)
        : Operand(MEM_REGISTER), register_index(register_index), displacement(displacement) {
    
}

ImmediateOperand::ImmediateOperand(uint16_t immediate)
        : Operand(IMMEDIATE), immediate(immediate)  {
    
}

AddressOperand::AddressOperand(uint16_t address)
        : Operand(MEM_IMMEDIATE), address(address)  {
    
}

int RegisterOperand::bitSize() const {
    return 2 + 3;
}

void RegisterOperand::writeBits(BitWriter<10> &bitWriter) const {
    bitWriter.writeBits(type, 2);
    bitWriter.writeBits(register_index, 3);
}

int MemRegisterOperand::bitSize() const {
    return 2 + 3 + 8;
}

void MemRegisterOperand::writeBits(BitWriter<10> &bitWriter) const {
    bitWriter.writeBits(type, 2);
    bitWriter.writeBits(register_index, 3);
    bitWriter.writeBits((uint8_t)displacement, 8);
}

int ImmediateOperand::bitSize() const {
    return 2 + 16;
}

void ImmediateOperand::writeBits(BitWriter<10> &bitWriter) const {
    bitWriter.writeBits(type, 2);
    bitWriter.writeBits(immediate, 16);
}

int AddressOperand::bitSize() const {
    return 2 + 16;
}

void AddressOperand::writeBits(BitWriter<10> &bitWriter) const {
    bitWriter.writeBits(type, 2);
    bitWriter.writeBits(address, 16);
}

int Instruction::getOperandCount() const {
    return Instruction::operationToOperandCount(operation);
}


uint8_t Instruction::encode(std::function<void(uint8_t)> byteSink) const {
    BitWriter<10> bw;
    
    bw.writeBits(operation, 5);
    
    int opCount = Instruction::operationToOperandCount(operation);
    assert(opCount >= 0);
    assert(opCount <= 2);
    if (opCount >= 1)
        source->writeBits(bw);
    if (opCount >= 2)
        target->writeBits(bw);
    bw.toSink(byteSink);
    
    return bw.getByteCount();
}

std::shared_ptr<Operand> decodeOperand(BitReader<10>& bitReader) {
    CPUInstructionOperandType operandType = bitReader.readBits<CPUInstructionOperandType>(2);
    switch(operandType) {
        case REGISTER: {
            uint8_t register_index = bitReader.readBits<uint8_t>(3);
            return std::shared_ptr<Operand>(new RegisterOperand(register_index));
        }
        case IMMEDIATE: {
            spg_register_t immediate = bitReader.readBits<spg_register_t>(sizeof(spg_register_t)*8);
            return std::shared_ptr<Operand>(new ImmediateOperand(immediate));
        }
        case MEM_REGISTER: {
            uint8_t register_index = bitReader.readBits<uint8_t>(3);
            int8_t displacement = bitReader.readBits<int8_t>(8);
            return std::shared_ptr<Operand>(new MemRegisterOperand(register_index, displacement));
        }
        case MEM_IMMEDIATE: {
            spg_addr_t address = bitReader.readBits<spg_addr_t>(sizeof(spg_addr_t)*8);
            return std::shared_ptr<Operand>(new AddressOperand(address));
        }
        default: throw DecodingError(std::string("Bad Instruction: Invalid Operand type: "+std::to_string(operandType)));
    }
}
    
Instruction Instruction::decode(std::function<uint8_t()> byteSource, size_t &bytes_read) {
    BitReader<10> bitReader(byteSource);
    
    Operation op = bitReader.readBits<Operation>(5);
    assert(op >= 0);
    assert(op <= RET);
    int operand_count = Instruction::operationToOperandCount(op);
    if (operand_count == 0) {
        bytes_read = bitReader.getBytesRead();
        return Instruction(op);
    }
    
    std::shared_ptr<Operand> source = decodeOperand(bitReader);
    if (operand_count == 1) {
        bytes_read = bitReader.getBytesRead();
        return Instruction(op, source);
    }
    if(operand_count > 2)
        throw DecodingError(std::string("Implementation bug in Instruction::operationToOperandCount: operand_count="+std::to_string(operand_count)));
    
    std::shared_ptr<Operand> target = decodeOperand(bitReader);
    bytes_read = bitReader.getBytesRead();
    return Instruction(op, source, target);
}

int Instruction::operationToOperandCount(Operation operation) {
    switch (operation) {
        case RET:
        case NOP: {
            return 0;
        }
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case SHR:
        case SHL:
        case CMP:
        case TEST:
        case MOV:
        case MOVB:{
            return 2;
        }
        case NOT:
        case JMP:
        case JE:
        case JNE:
        case CALL:
        case PUSH:
        case INC:
        case DEC:
        case POP:{
            return 1;
        }
        default: {
            throw DecodingError(std::string("operationToOperandCount does not support operation "+std::to_string(operation)));
        }
    }
}

#pragma clang diagnostic pop
