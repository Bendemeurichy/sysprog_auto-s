#include "Mem.h"

//TODO

Mem::Mem(spg_addr_t start,spg_addr_t stop) : Module(start,stop) {
    //TODO
}


bool Mem::manages(const spg_addr_t& address) const {
    //TODO
    return false;
}

uint8_t Mem::read1(const spg_addr_t &address) {
    //TODO
    return 0;
}

spg_register_t Mem::read2_be(const spg_addr_t &address) {
    //TODO
    return 0;
}

void Mem::write1(const spg_addr_t& address, uint8_t val) {
    //TODO
}

void Mem::write2_be(const spg_addr_t& address, spg_register_t val) {
    //TODO
}

spg_addr_t Mem::getStart() const {
    //TODO
    return 0;
}

spg_addr_t Mem::getEnd() const {
    //TODO
    return 0;
}

size_t Mem::getLen() const {
    //TODO
    return 0;
}

