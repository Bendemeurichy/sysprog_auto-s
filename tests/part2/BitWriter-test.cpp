
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <assert.h>
#include <iostream>

#include "../../game/carbrain/part2/Assembler.h"
#include "../../game/carbrain/part2/CPUInstruction.h"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(BitWriterTests)

BOOST_AUTO_TEST_CASE( BitWriter_A ) {
    std::cout << "Test A:" << std::endl;
    BitWriter<4> bw;
    
    uint8_t a = 0xA5;
    bw.writeBits(a, 4);
    a = 0xF3;
    bw.writeBits(a, 4);
    bw.writeBits(0x0F, 4);
    bw.writeBits(0x07, 4);
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 2  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0x53  );
    BOOST_CHECK_EQUAL( bw.getByte(1) , 0xF7  );
}


BOOST_AUTO_TEST_CASE( BitWriter_B ) {
    std::cout << "Test B:" << std::endl;
    BitWriter<4> bw;
    
    uint16_t a = 0x1234;
    bw.writeBits(a, 12);
    a = 0x5678;
    bw.writeBits(a, 12);
    bw.writeBits(0xAF, 4);
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 4  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0x23  );
    BOOST_CHECK_EQUAL( bw.getByte(1) , 0x46  );
    BOOST_CHECK_EQUAL( bw.getByte(2) , 0x78  );
    BOOST_CHECK_EQUAL( bw.getByte(3) , 0xF0  );
}

BOOST_AUTO_TEST_CASE( BitWriter_C ) {
    std::cout << "Test C:" << std::endl;
    BitWriter<4> bw;
    
    bw.writeBits(0x2, 2); //10
    bw.writeBits(0x3, 2); //11  B
    bw.writeBits(0x0, 2); //00
    bw.writeBits(0x1, 1); //10  2
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 1  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0xB2  );
}

BOOST_AUTO_TEST_CASE( BitWriter_D ) {
    std::cout << "Test D:" << std::endl;
    BitWriter<4> bw;
    
    uint8_t a = 0x1;
    bw.writeBits(a, 1);
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 1  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0x80  );
}

BOOST_AUTO_TEST_CASE( BitWriter_E ) {
    std::cout << "Test E:" << std::endl;
    BitWriter<10> bw;
    bw.writeBits(0x4, 5); //00100
    bw.writeBits(0x3, 2); //11
    bw.writeBits(0x4, 3); //100
    bw.writeBits((uint8_t)0x7, 8); //00000111
    bw.writeBits(0x0, 2); //00
    bw.writeBits(0x88, 16); //00000000 0100 0100

//0010 0111 0000 0001 1100 0000 0000 0100 0100
//   2    7    0    1    C    0    0    8    8
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 5  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0x27  );
    BOOST_CHECK_EQUAL( bw.getByte(1) , 0x01  );
    BOOST_CHECK_EQUAL( bw.getByte(2) , 0xC0  );
    BOOST_CHECK_EQUAL( bw.getByte(3) , 0x08  );
    BOOST_CHECK_EQUAL( bw.getByte(4) , 0x80  );
}

BOOST_AUTO_TEST_CASE( BitWriter_F ) {
    std::cout << "Test F:" << std::endl;
    BitWriter<4> bw;
    
    uint8_t a = 0x0;
    uint8_t b = 0x1;
    bw.writeBits(a, 7);
    bw.writeBits(b, 1);
    
    BOOST_CHECK_EQUAL( bw.getByteCount() , 1  );
    BOOST_CHECK_EQUAL( bw.getByte(0) , 0x1  );
}

BOOST_AUTO_TEST_SUITE_END()
