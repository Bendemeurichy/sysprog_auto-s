#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BusTest)

BOOST_AUTO_TEST_CASE( dummytest ) {
    BOOST_CHECK_EQUAL(1,1);
    //Create a bus, and test writing and reading from different addresses.
    //See if the correct modules are written to/read from.
}

BOOST_AUTO_TEST_SUITE_END()
