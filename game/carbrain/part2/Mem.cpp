#include "Mem.h"

//TODO

Mem::Mem(spg_addr_t start, spg_addr_t end) : Module(start, end) {
    data.resize(len_byte);
}

bool Mem::manages(const spg_addr_t &address) const {
    //TODO
    return address >= start && address <= end;
}

uint8_t Mem::read1(const spg_addr_t &address) {
    //TODO
    if(!manages(address)){
        throw ModuleError("Invalid address");
    }
    return (data[address-start]);
}


void Mem::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if(!manages(address)){
        throw ModuleError("Invalid address");
    }
    data[address-start] = val;
}

