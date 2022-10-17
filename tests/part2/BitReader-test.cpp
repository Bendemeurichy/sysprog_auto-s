#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <string>
#include <assert.h>

#include "../../game/carbrain/part2/Assembler.h"
#include "../../game/carbrain/part2/CPUInstruction.h"

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(BitReaderTests)

BOOST_AUTO_TEST_CASE( Bitreader_can_read_bits ) {
    
    uint8_t bytes[8] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
    //0001001000110100010101100111100010011010101111001101111011110000
    BOOST_CHECK_EQUAL(bytes[0], 0x12);
    BOOST_CHECK_EQUAL(bytes[1], 0x34);
    
    // 3     5   3    10            22                1 1 1 1 1 1   15 left
    //000 10010 001 1010001010 1100111100010011010101 1 1 1 0 0 1 101111011110000
    BitReader<10> bitReader([bytes](){ static int index = 0; return bytes[index++]; });
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(3) , 0x0  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  3  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 1  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(5) , 0x12  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  8  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 1  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(3) , 0x01  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  11  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 2  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint16_t>(10) , 0x28A  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  21  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 3  );                // 3    3    C    4    D    5
    BOOST_CHECK_EQUAL( bitReader.readBits<uint32_t>(22) , 0x33C4D5  ); //11 0011 1100 0100 1101 0101
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  43  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x1  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  44  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x1  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  45  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x1  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  46  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x0  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  47  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x0  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  48  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 6  );
    BOOST_CHECK_EQUAL( bitReader.readBits<uint8_t>(1) , 0x1  );
    BOOST_CHECK_EQUAL( bitReader.getBitsRead() ,  49  );
    BOOST_CHECK_EQUAL( bitReader.getBytesRead() , 7  );
}

BOOST_AUTO_TEST_SUITE_END()
