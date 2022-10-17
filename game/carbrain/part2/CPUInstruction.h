#include <utility>

#ifndef SYSPROG_CPUINSTRUCTION_H
#define SYSPROG_CPUINSTRUCTION_H

#include "Constants.h"
#include "BitHelper.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <cassert>

//register 0        -> %pc == $ip  (program counter aka instruction pointer)
//register 1        -> %sp         (stack pointer)
//skipped on this CPU: %bp         (base pointer)
#define REG_IP 0
#define REG_SP 1
#define REG_A 2
#define REG_B 3
#define REG_C 4
#define REG_D 5
#define REG_E 6
#define REG_F 7

namespace CPUInstruction {
    typedef enum CPUInstructionOperandType {
        IMMEDIATE = 0, REGISTER = 1, MEM_IMMEDIATE = 2, MEM_REGISTER = 3
    } CPUInstructionOperandType;

    typedef enum Operation {
        NOP = 0,
        MOV = 1, MOVB = 2,
        PUSH = 3, POP = 4,
        ADD = 5, SUB = 6, AND = 7, OR = 8, XOR = 9,
        NOT = 10, SHR = 11, SHL = 12, INC = 13, DEC = 14,
        CMP = 15, TEST = 16,
        JMP = 17, JE = 18, JNE = 19,
        CALL = 20, RET = 21
    } CPUInstructionType;

    class Operand {
    public:
        const CPUInstructionOperandType type;
    
        [[nodiscard]] virtual int bitSize() const = 0;
        virtual void writeBits(BitWriter<10>& bitWriter) const = 0;
    protected:
        explicit Operand(CPUInstructionOperandType type);
    };
    
    class RegisterOperand : public Operand {
    public:
        const uint8_t register_index;       //3 bit
        explicit RegisterOperand(uint8_t register_index);
        [[nodiscard]] int bitSize() const override;
        void writeBits(BitWriter<10>& bitWriter) const override;
    };
    
    class MemRegisterOperand : public Operand {
    public:
        const uint8_t register_index;       //3 bit
        const int8_t displacement; //+ or - offset against base mem address  (8 bit)
        MemRegisterOperand(uint8_t register_index, int8_t displacement);
        [[nodiscard]] int bitSize() const override;
        void writeBits(BitWriter<10>& bitWriter) const override;
    };
    
    class ImmediateOperand : public Operand {
    public:
        const uint16_t immediate;     //16 bit
        explicit ImmediateOperand(uint16_t immediate);
        [[nodiscard]] int bitSize() const override;
        void writeBits(BitWriter<10>& bitWriter) const override;
    };
    
    //2 + 16      //6 bit left
    class AddressOperand : public Operand {
    public:
        const uint16_t address;       //16 bit
        explicit AddressOperand(uint16_t address);
        [[nodiscard]] int bitSize() const override;
        void writeBits(BitWriter<10>& bitWriter) const override;
    };
    
    /**
     * The instruction class can be useful as an intermediate format between:
     *   - assembler code (text) and binary CPU instructions
     *   - binary CPU instructions and the execution of them on the CPU
     *
     * You are not required to use this class.
     */
    class Instruction {
    public:
        const Operation operation;     //5 bit
        const std::shared_ptr<Operand> source;  //variable size, not byte-rounded
        const std::shared_ptr<Operand> target;  //variable size, not byte-rounded
        //always padded (to round byte number) with 0's
        
        Instruction(const Operation &operation, std::shared_ptr<Operand> source, std::shared_ptr<Operand> target);
        Instruction(const Operation &operation, std::shared_ptr<Operand> operand);
        explicit Instruction(const Operation &operation);
        
        [[nodiscard]] int getOperandCount() const;
        static int operationToOperandCount(Operation operation);
        
        /**
         * @param byteSink a lambda used to write the bytes
         * @param bytes_read a reference to a variable. It will be modified to contain the number of bytes read.
         * @return the number of bytes consumed (= number of bytes of the written instruction)
         */
        uint8_t encode(std::function<void(uint8_t)> byteSink) const;
        
        /**
         * @param byteSource a lambda used to read bytes
         * @param bytes_read (return value) returns the number of bytes read from the byteSource (= number of bytes of the read instruction)
         * @return the read CPU Instruction
         */
        static Instruction decode(std::function<uint8_t()> byteSource, size_t &bytes_read);
    };
}


#endif //SYSPROG_CPUINSTRUCTION_H
