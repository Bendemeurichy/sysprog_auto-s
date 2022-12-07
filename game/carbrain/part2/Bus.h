#ifndef SYSPROG_BUS_H
#define SYSPROG_BUS_H


#include <cstdint>
#include <memory>
#include <vector>
#include "Constants.h"

class Bus;
#include "Module.h"
#include "CPU.h"
#include "Mem.h"

class Bus {    
    //TODO: Hou in deze klasse een lijst BotModules bij. 
    //      Doe dit generiek: Ga er niet van uit dat Bus op voorhand weet welke modules 
    //      op welk geheugenadres beschikbaar gemaakt zullen worden.
    std::vector<std::shared_ptr<Module>> modules;
    std::shared_ptr<Mem> stack_Mem;
    std::shared_ptr<Mem> code_Mem;

public:
    explicit Bus(std::shared_ptr<Mem> code_Mem, std::shared_ptr<Mem> stack_Mem);
    
    //the bus should check if addresses are aligned, and raise an exception if they are not
    //alignment takes byte_count into account of course
    //it will also raise an interrupt if byte_count is more than allowed (thus > sizeof(spg_register_t))

    //TODO: Implementeer methodes om 1 of 2 bytes te lezen en schrijven van een bepaald geheugenadres. 
    //      De Bus zal dan a.d.h.v het geheugenadres de juiste module contacteren. 
    //      Deze methodes worden door de CPU opgeroepen. 
    //      De CPU zal nooit rechtstreeks een BotModule aanspreken.

    uint8_t read1(const spg_addr_t& address);
    //result read2_be is assumed to be in BIG ENDIAN order
    spg_register_t read2_be(const spg_addr_t& address);

    void write1(const spg_addr_t& address, uint8_t val);
    //write2_be val is assumed to be in BIG ENDIAN order
    void write2_be(const spg_addr_t& address, spg_register_t val);

    //Voeg een BotModule toe aan de Bus
    void add_module(const std::shared_ptr<Module>&);
};

#endif //SYSPROG_BUS_H
