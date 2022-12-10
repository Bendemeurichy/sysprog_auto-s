#include "Bus.h"
#include "Exceptions.h"

//TODO

Bus::Bus(){
    //TODO
    modules = std::vector<std::shared_ptr<Module>>();
}

uint8_t Bus::read1(const spg_addr_t &address) {
    if (!is_valid_address(address)){
        throw ModuleError("Invalid address");
    }
    for (auto module : modules){
        if (module->manages(address)){
            return module->read1(address);
        }
    }
    return 0;
}

spg_register_t Bus::read2_be(const spg_addr_t &address) {
    //TODO
    if (!is_valid_address(address)){
        throw ModuleError("Invalid address");
    }
    for (auto module : modules){
        if (module->manages(address)){
            return module->read2_be(address);
        }
    }
    return 0;
}

void Bus::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if (!is_valid_address(address)){
        throw ModuleError("Invalid address");
    }
    for (auto module : modules){
        if (module->manages(address)){
            module->write1(address, val);
        }
    }
}

void Bus::write2_be(const spg_addr_t &address, spg_register_t val) {
    //TODO
    if (!is_valid_address(address)){
        throw ModuleError("Invalid address");
    }
    for (auto module : modules){
        if (module->manages(address)){
            module->write2_be(address, val);
        }
    }
}

void Bus::add_module(const std::shared_ptr<Module>& module) {
    modules.push_back(module);
}

bool Bus::is_valid_address(const spg_addr_t& address){
    for (auto module : modules){
        if (module->manages(address)){
            return true;
        }
    }
    return false;
}
