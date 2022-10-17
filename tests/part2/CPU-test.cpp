#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <assert.h>

#include "../../game/carbrain/part2/CPU.h"
#include "../../game/carbrain/part2/Assembler.h"
#include "../../game/carbrain/part2/CPUInstruction.h"
#include "../../game/carbrain/part2/Board.h"

#include <netinet/in.h>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(CpuTests)

    BOOST_AUTO_TEST_CASE( CPU_MOV_reg_instruction ) {
        // First the value 0x1234 is written to register a.
        // Then the value in register a is written to register b.
        // The final result is should be that value 0x1234 is saved in register a and b.

        Board board(200, 300);
        std::vector<std::string> asm_lines;
        asm_lines.emplace_back("MOV $0x1234, %a");
        asm_lines.emplace_back("MOV %a, %b");
        board.loadAndStartCodeFromAsmLines(asm_lines);
        std::shared_ptr<CPU> cpu = board.getCpu();
        cpu->tick();
        cpu->tick();
        BOOST_CHECK_EQUAL( cpu->getRegisterValue(REG_A) , 0x1234  );
        BOOST_CHECK_EQUAL( cpu->getRegisterValue(REG_B) , 0x1234  );
    }

    BOOST_AUTO_TEST_CASE( CPU_MOV_mem_instruction ) {
        // Write the value of the address where the stack pointer points into register a.
        // Write the value 0x1234 to the top of the stack. The top of the stack is the default position of the stack pointer.
        // The result should be that the value 0x1234 is stored in register a.

        Board board(200, 300);

        std::vector<std::string> asm_lines;
        asm_lines.emplace_back("MOV (%sp), %a");
        board.loadAndStartCodeFromAsmLines(asm_lines);

        std::shared_ptr<CPU> cpu = board.getCpu();

        //Stack pointer should point to top of the stack after loadAndStartCode*
        spg_addr_t spValue = cpu->getRegisterValue(REG_SP);

        //write a value to the top of the stack, using the bus
        board.getBus()->write2_be(spValue, htons(0x1234));

        //Check if it's written correctly
        BOOST_CHECK_EQUAL( board.getBus()->read2_be(spValue) , ntohs(0x1234) );

        //check if CPU can read it correctly
        cpu->tick();
        BOOST_CHECK_EQUAL( cpu->getRegisterValue(REG_A) , 0x1234  );
    }
    
    //TODO add tests for each CPU instruction
    //TODO add separate tests for each operand type (for example, using MOV)

BOOST_AUTO_TEST_SUITE_END()
