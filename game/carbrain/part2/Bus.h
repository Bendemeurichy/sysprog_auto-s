#ifndef SYSPROG_BUS_H
#define SYSPROG_BUS_H


#include <cstdint>
#include <memory>
#include <vector>
#include "Constants.h"

class Bus;
#include "Module.h"
#include "CPU.h"

class Bus {    
    //TODO: Hou in deze klasse een lijst BotModules bij. 
    //      Doe dit generiek: Ga er niet van uit dat Bus op voorhand weet welke modules 
    //      op welk geheugenadres beschikbaar gemaakt zullen worden.
    

public:
    explicit Bus();
    
    //Er zijn verschillende modules waaruit geheugen kan uitgelezen worden.
    //Als een read of write niet geheel binnen 1 module valt, gooi je een exception.
    //Je hoeft m.a.w. geen logica te implementeren voor read/writes over meerdere modules tegelijk.

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
