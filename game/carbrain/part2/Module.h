#ifndef SYSPROG_MODULE_H
#define SYSPROG_MODULE_H

#include "Constants.h"

class Module;
#include "Bus.h"
#include <iostream>
#include <cstdint>
#include <memory>

class Module {
protected:
    const spg_addr_t start; //inclusive
    const spg_addr_t end;   //exclusive !
    const size_t len_byte;
    std::vector<uint8_t> data;
public:
    Module(spg_addr_t start, spg_addr_t end);
    virtual ~Module() = default;
    
    virtual bool manages(const spg_addr_t& address) const;
    
    virtual uint8_t read1(const spg_addr_t &address);
    virtual spg_register_t read2_be(const spg_addr_t &address);
    virtual void write1(const spg_addr_t& address, uint8_t val);
    virtual void write2_be(const spg_addr_t& address, spg_register_t val);

    virtual spg_addr_t getStart() const;
    virtual spg_addr_t getEnd() const;
    virtual size_t getLen() const;

};


#endif //SYSPROG_MODULE_H
