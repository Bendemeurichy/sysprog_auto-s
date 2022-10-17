
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <assert.h>

#include "../../game/carbrain/part2/Assembler.h"
#include "../../game/carbrain/part2/CPUInstruction.h"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(InstructionEncodeTests)
    
    using namespace CPUInstruction;
    
BOOST_AUTO_TEST_CASE( Encode_instructions_to_binary_mov ) {
    uint8_t code[100] = {0};
    Assembler assembler((spg_addr_t) 0x10);
    assembler.labels["BLAH"] = 50;
    assembler.labels["blih"] = 60;
    assembler.constants["foo"] = 9;
    assembler.constants["BAR"] = 10;
    
    assembler.target = code;
    assembler.cur_target = code;
    assembler.target_size = 100;
    memset(code, 0xFF, sizeof(code));
    assembler.write_instruction(Instruction(
            MOV, //MOV (1) (5 bit)
            std::shared_ptr<Operand>(new RegisterOperand(1)), //REGISTER (1) - val=1     (2 + 3 bit)
            std::shared_ptr<Operand>(new RegisterOperand(2))  //REGISTER (1) - val=2     (2 + 3 bit)
            //1 bit padding
    ));
    
    //00001 01 001 01 010 0
    //0   A     5     4
    
    BOOST_CHECK_EQUAL(code[0], 0x0A);
    BOOST_CHECK_EQUAL(code[1], 0x54);
    
    for (size_t i = 5; i < sizeof(code); i++) BOOST_CHECK_EQUAL(code[i], 0xFF);
    
    
}
BOOST_AUTO_TEST_CASE( Encode_instructions_to_binary_add ) {
    uint8_t code[100] = { 0 };
    Assembler assembler((spg_addr_t) 0x10);
    assembler.labels["BLAH"] = 50;
    assembler.labels["blih"] = 60;
    assembler.constants["foo"] = 9;
    assembler.constants["BAR"] = 10;
    
    assembler.target = code;
    assembler.cur_target = code;
    memset(code, 0xFF, sizeof(code));
    assembler.write_instruction(Instruction(
            ADD,//ADD (6) (5 bit)
            std::shared_ptr<Operand>(new ImmediateOperand(0x88)), //IMMEDIATE (0) (2bit) - val=0x88 (16 bit)
            std::shared_ptr<Operand>(new MemRegisterOperand(4, 7)) //MEM_REGISTER (3) (2bit) - reg_index=4 (3 bit) - disp (7) (8bit)
    ));
    
    // 00101 00 00000000 10001000 11 100 00000111
    //
    //0010 1000 0000 0001 0001 0001 1100 0000 0111
    //   2    8    0    1    1    1    C    0    7
    
    BOOST_CHECK_EQUAL( code[0] , 0x28  );
    BOOST_CHECK_EQUAL( code[1] , 0x01  );
    BOOST_CHECK_EQUAL( code[2] , 0x11  );
    BOOST_CHECK_EQUAL( code[3] , 0xC0  );
    BOOST_CHECK_EQUAL( code[4] , 0x70  );
    for (size_t i = 5; i < sizeof(code); i++) BOOST_CHECK_EQUAL( code[i] , 0xFF  );
    assert(assembler.cur_target - assembler.target == 5);
    
    
}
BOOST_AUTO_TEST_CASE( Encode_instructions_to_binary_sub ) {
    uint8_t code[100] = { 0 };
    Assembler assembler((spg_addr_t) 0x10);
    assembler.labels["BLAH"] = 50;
    assembler.labels["blih"] = 60;
    assembler.constants["foo"] = 9;
    assembler.constants["BAR"] = 10;
    
    assembler.target = code;
    assembler.cur_target = code;
    memset(code, 0xFF, sizeof(code));
    assembler.write_instruction(Instruction(SUB,
                                            std::shared_ptr<Operand>(new ImmediateOperand(42)),
                                            std::shared_ptr<Operand>(new AddressOperand(0x1234))
                                )
    );
    
    // SUB  IMMEDIATE 42                MEM_IMM  0x1234
    //00110    00     0000000000101010    10     0001001000110100
    
    //0011 0000 0000 0000 0101 0101 0000 1001 0001 1010 0
    //   3    0    0    0    5    5    0    9    1    A 0
    
    
    BOOST_CHECK_EQUAL( code[0] , 0x30  );
    BOOST_CHECK_EQUAL( code[1] , 0x00  );
    BOOST_CHECK_EQUAL( code[2] , 0x55  );
    BOOST_CHECK_EQUAL( code[3] , 0x09  );
    BOOST_CHECK_EQUAL( code[4] , 0x1A  );
    BOOST_CHECK_EQUAL( code[5] , 0x00  );
    for (size_t i = 6; i < sizeof(code); i++) BOOST_CHECK_EQUAL( code[i] , 0xFF  );
    assert(assembler.cur_target - assembler.target == 6);
    
    
}
BOOST_AUTO_TEST_CASE( Encode_instructions_to_binary_nop ) {
    uint8_t code[100] = { 0 };
    Assembler assembler((spg_addr_t) 0x10);
    assembler.labels["BLAH"] = 50;
    assembler.labels["blih"] = 60;
    assembler.constants["foo"] = 9;
    assembler.constants["BAR"] = 10;
    
    assembler.target = code;
    assembler.cur_target = code;
    memset(code, 0xFF, sizeof(code));
    assembler.write_instruction(Instruction(NOP));
    BOOST_CHECK_EQUAL( code[0] , 0x0  );
    for (size_t i = 1; i < sizeof(code); i++) BOOST_CHECK_EQUAL( code[i] , 0xFF  );
}

BOOST_AUTO_TEST_SUITE_END()
