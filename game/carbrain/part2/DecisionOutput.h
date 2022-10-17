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
    DecisionOutput(const spg_addr_t start, uint8_t max_decisions);

    void write1(const spg_addr_t &address, uint8_t val) override;
    
    CarBrainDecision popDecision() override;
    bool isDecisionAvailable() override;
private:
    //TODO
};

#endif //SYSPROG_DECISION_H
