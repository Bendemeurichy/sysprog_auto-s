#include "Bus.h"

//TODO

Bus::Bus() {
    //TODO
}

uint8_t Bus::read1(const spg_addr_t &address) {
    //TODO
    return 0;
}

spg_register_t Bus::read2_be(const spg_addr_t &address) {
    //TODO
    return 0;
}

void Bus::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
}

void Bus::write2_be(const spg_addr_t &address, spg_register_t val) {
    //TODO
}

void Bus::add_module(const std::shared_ptr<Module>&) {
    //TODO
}
