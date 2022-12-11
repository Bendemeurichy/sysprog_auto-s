#ifndef SYSPROG_MEM_H
#define SYSPROG_MEM_H

#include <cstdio>
#include <assert.h>
#include "Module.h"
#include "Constants.h"
#include "Exceptions.h"

class Mem : public Module {
    public:
    Mem(spg_addr_t start, spg_addr_t end);
    //TODO Implementeer een constructor, die (o.a.) de grootte van dit blok geheugen mee krijgt.
    //TODO Implementeer de virtuele methoden van BotModule, om 1 of 2 bytes te lezen en schrijven uit/naar dit blok geheugen.

    ~Mem() = default;
    bool manages(const spg_addr_t& address) const;
    
    uint8_t read1(const spg_addr_t &address);
        
    void write1(const spg_addr_t& address, uint8_t val);
    void setData(std::vector<uint8_t> data, int size);
};

#endif //SYSPROG_MEM_H
