#include "Mem.h"

//TODO

Mem::Mem(spg_addr_t start, spg_addr_t end) : Module(start, end) {
    data.resize(len_byte);
}

bool Mem::manages(const spg_addr_t &address) const {
    //TODO
    return address >= start && address < end;
}

uint8_t Mem::read1(const spg_addr_t &address) {
    //TODO
    if(!manages(address)){
        throw ModuleError("Invalid address at address: " + std::to_string(address));
    }
    return (data[address-start]);
}


void Mem::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if(!manages(address)){
        throw ModuleError("Invalid address at address: " + std::to_string(address));
    }
    data[address-start] = val;
}

void Mem::setData(std::vector<uint8_t> data, int size) {
    //TODO
    if(size > len_byte){
        throw ModuleError("Invalid size");
    }
    for(int i = 0; i < size; i++){
        this->data[i] = data[i];
    }
}

