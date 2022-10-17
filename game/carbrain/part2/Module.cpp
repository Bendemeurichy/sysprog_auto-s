#include "Module.h"
#include "Exceptions.h"

#include <netinet/in.h>

Module::Module(spg_addr_t start, spg_addr_t end) :
        start(start), end(end), len_byte(end-start) {
    //nothing to do here
}


bool Module::manages(const spg_addr_t &address) const {
    return address >= start && address < end;
}


spg_register_t Module::read2_be(const spg_addr_t &address) {
    //default implementation, that can be overwritten by classes inheriting from Module.
    //TODO implement this by using read1 twice.
    //     That way, no specific module implementation (=subclass)  will need to implement read2_be itself.
    return 0;
}

void Module::write2_be(const spg_addr_t& address, spg_register_t val) {
    //default implementation, that can be overwritten by classes inheriting from Module.
    //TODO implement this by calling write1 twice.
    //     That way, no specific module implementation (=subclass) will need to implement write2_be itself.
}

uint8_t Module::read1(const spg_addr_t &address) {
    //default implementation: needs to be overwritten by classes inheriting from Module if they allow reading.
    throw ReadNotAllowedError();
}

void Module::write1(const spg_addr_t &address, uint8_t val) {
    //default implementation: needs to be overwritten by classes inheriting from Module if they allow writing.
    throw WriteNotAllowedError();
}

spg_addr_t Module::getStart() const {
    return start;
}

spg_addr_t Module::getEnd() const {
    return end;
}

size_t Module::getLen() const {
    return len_byte;
}
