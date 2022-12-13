#include "DecisionOutput.h"
#include "Exceptions.h"

DecisionOutput::DecisionOutput() : Module(IO_DECISION_START, IO_DECISION_START+IO_DECISION_SIZE)
{
    // TODO
}

uint8_t DecisionOutput::read1(const spg_addr_t &address) {
    //std::cout << "read1 decisions on address "<< std::to_string(address)  <<std::endl;
        throw ReadNotAllowedError();
}

spg_register_t DecisionOutput::read2_be(const spg_addr_t &address) {
    //std::cout<<"read2_be decisions on address "<< std::to_string(address)  <<std::endl;
    throw ReadNotAllowedError();
}

void DecisionOutput::write1(const spg_addr_t &address, uint8_t val) {
    //TODO
    if (!manages(address)) {
        throw ModuleError("DecisionOutput can't write to address " + std::to_string(address));
    }
    if((address-start)==0){
        decisionCount=val;
        decisionIndex=0;

    } else {
    std::cout<<"write1 decisions on address "<< std::to_string(address-start-1) << " to " << std::to_string(val) <<std::endl;
    decisions[(address-start)-1] = static_cast<CarBrainDecision>(val);
    }
}

CarBrainDecision DecisionOutput::popDecision()
{
    if(isDecisionAvailable()){
        
        CarBrainDecision decision = decisions[decisionIndex];
        decisionIndex++;
        std::cout << "[" << std::to_string(decisions[0])<< std::to_string(decisions[1]) << std::to_string(decisions[2]) << std::to_string(decisions[3]) << std::to_string(decisions[4]) << std::to_string(decisions[5]) << std::to_string(decisions[6]) << std::to_string(decisions[7]) << std::to_string(decisions[8]) << std::to_string(decisions[9]) << "]" << std::endl;
        std::cout<<"popDecision decisions  "<< std::to_string(decision) <<std::endl;
        return decision;
    } else {
        throw ModuleError("No decision available");
    }
}

bool DecisionOutput::isDecisionAvailable() {
    return decisionCount > decisionIndex;
}
