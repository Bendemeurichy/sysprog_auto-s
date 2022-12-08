#include "Mem.h"

//TODO

Mem::Mem(spg_addr_t start, spg_addr_t end) : Module(start, end) {
    //TODO
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
    return 0;
}


void Mem::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if(!manages(address)){
        throw ModuleError("Invalid address");
    }
    
}

spg_addr_t Mem::getStart() const {
    return start;
}

spg_addr_t Mem::getEnd() const {
    return end;
}

size_t Mem::getLen() const {
    return len_byte;
}

