#include "DecisionOutput.h"
#include "Exceptions.h"

DecisionOutput::DecisionOutput() : Module(IO_DECISION_START, IO_DECISION_START+IO_DECISION_SIZE)
{
    // TODO
}

uint8_t DecisionOutput::read1(const spg_addr_t &address) {
    throw ModuleError("can't read from DecisionOutput");
}

spg_register_t DecisionOutput::read2_be(const spg_addr_t &address) {
    throw ModuleError("can't read from DecisionOutput");
}

void DecisionOutput::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if (!manages(address)) {
        throw ModuleError("DecisionOutput can't write to address " + std::to_string(address));
    }
    decisions[address-start] = static_cast<CarBrainDecision>(val);
}

CarBrainDecision DecisionOutput::popDecision()
{
    if(isDecisionAvailable()){
        
        CarBrainDecision decision = decisions[1];
        for(int i=1;i<decisions[0];i++){
            decisions[i]=decisions[i+1];
        }
        decisions[decisions[0]]=DECISION__COUNT;
        decisions[0] = static_cast<CarBrainDecision>(decisions[0] - 1);
        return decision;
    } else {
        throw ModuleError("No decision available");
    }
}

bool DecisionOutput::isDecisionAvailable() {
    return decisions[0] > 0;
}
