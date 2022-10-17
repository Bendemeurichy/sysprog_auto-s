#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <assert.h>

#include "../../game/carbrain/part2/Assembler.h"
#include "../../game/carbrain/part2/CPUInstruction.h"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(InstructionDecodeTests)

using namespace CPUInstruction;

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

BOOST_AUTO_TEST_CASE( Decode_binary_instructions_mov ) {
    uint8_t code[100] = { 0 };
    
    Assembler assembler((spg_addr_t) 0x10);
    
    size_t bytes_read;
    
    std::vector<std::string> lines;
    lines.emplace_back("MOV %sp, ($201)");
    assembler.assemble(lines, code, 100);
    
    Instruction inst = Instruction::decode([&code]() { static int index = 0; return code[index++]; }, bytes_read);
    BOOST_CHECK_EQUAL( bytes_read , 4  ); //bits: 5 + 2 + 3 + 2 + 16 -> 28
    BOOST_CHECK_EQUAL( inst.operation , MOV  );
    BOOST_CHECK_EQUAL( inst.getOperandCount() , 2  );
    BOOST_CHECK_EQUAL( inst.source->type , REGISTER  );
    BOOST_CHECK_EQUAL( std::dynamic_pointer_cast<RegisterOperand>(inst.source)->register_index , 1  );
    BOOST_CHECK_EQUAL( inst.target->type , MEM_IMMEDIATE  );
    BOOST_CHECK_EQUAL( std::dynamic_pointer_cast<AddressOperand>(inst.target)->address , 201  );
}
    
BOOST_AUTO_TEST_CASE( Decode_binary_instructions_nop ) {
    uint8_t code[100] = { 0 };
    
    Assembler assembler((spg_addr_t) 0x10);
    
    size_t bytes_read;
    std::vector<std::string> lines;
    lines.emplace_back("NOP");
    assembler.assemble(lines, code, 100);
    
    Instruction inst = Instruction::decode([&code]() { static int index = 0; return code[index++]; }, bytes_read);
    BOOST_CHECK_EQUAL( bytes_read , 1  );
    BOOST_CHECK_EQUAL( inst.operation , NOP  );
    BOOST_CHECK_EQUAL( inst.getOperandCount() , 0  );
}
    
BOOST_AUTO_TEST_CASE( Decode_binary_instructions_mov2 ) {
    uint8_t code[100] = { 0 };
    
    Assembler assembler((spg_addr_t) 0x10);
    
    size_t bytes_read;
    std::vector<std::string> lines;
        lines.emplace_back("MOV -8(%b), $0xFFEE1122");
    assembler.assemble(lines, code, 100);
    
    Instruction inst = Instruction::decode([&code]() { static int index = 0; return code[index++]; }, bytes_read);
    BOOST_CHECK_EQUAL( bytes_read , 5  ); //bits: 5 + 2 + 3 + 8 + 2 + 16 = 36
    BOOST_CHECK_EQUAL( inst.operation , MOV  );
    BOOST_CHECK_EQUAL( inst.getOperandCount() , 2  );
    BOOST_CHECK_EQUAL( inst.source->type , MEM_REGISTER  );
        BOOST_CHECK_EQUAL( std::dynamic_pointer_cast<MemRegisterOperand>(inst.source)->register_index , REG_B  );
    BOOST_CHECK_EQUAL( std::dynamic_pointer_cast<MemRegisterOperand>(inst.source)->displacement , -8  );
    BOOST_CHECK_EQUAL( inst.target->type , IMMEDIATE  );
    BOOST_CHECK_EQUAL( (std::dynamic_pointer_cast<ImmediateOperand>(inst.target)->immediate & 0xFFFF) , 0x1122  );
}
    
BOOST_AUTO_TEST_CASE( Decode_binary_instructions_pop ) {
    uint8_t code[100] = { 0 };
    
    Assembler assembler((spg_addr_t) 0x10);
    
    size_t bytes_read;
    std::vector<std::string> lines;
    lines.emplace_back("POP ($0x88)");
    assembler.assemble(lines, code, 100);
    
    Instruction inst = Instruction::decode([&code]() { static int index = 0; return code[index++]; }, bytes_read);
    BOOST_CHECK_EQUAL( bytes_read , 3  ); // bits: 5 + 2 + 16 = 23
    BOOST_CHECK_EQUAL( inst.operation , POP  );
    BOOST_CHECK_EQUAL( inst.getOperandCount() , 1  );
    BOOST_CHECK_EQUAL( inst.source->type , MEM_IMMEDIATE  );
    BOOST_CHECK_EQUAL( std::dynamic_pointer_cast<AddressOperand>(inst.source)->address , 0x88  );
}


BOOST_AUTO_TEST_SUITE_END()
