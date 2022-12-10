#ifndef SYSPROG_DECISION_H
#define SYSPROG_DECISION_H

#include <cstdint>
#include "../carbrain_es_integration.h"

#include "Module.h"
#include "BitHelper.h"

class DecisionOutputSource {
public:
    virtual ~DecisionOutputSource() = default;
    virtual CarBrainDecision popDecision() = 0;
    virtual bool isDecisionAvailable() = 0;
};

//maximum allowed for max_decisions parameter
#define MAX_MAX_DECISIONS 10

class DecisionOutput : public Module, public DecisionOutputSource {
public:
    DecisionOutput();

    uint8_t read1(const spg_addr_t &address) override;
    spg_register_t read2_be(const spg_addr_t &address) override;

    void write1(const spg_addr_t &address, uint8_t val) override;
    void write2_be(const spg_addr_t &address, spg_register_t val) override;
    
    CarBrainDecision popDecision() override;
    bool isDecisionAvailable() override;

private:
    //TODO
};

#endif //SYSPROG_DECISION_H
