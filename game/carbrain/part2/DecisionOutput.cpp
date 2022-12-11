#include "DecisionOutput.h"
#include "Exceptions.h"

DecisionOutput::DecisionOutput() : Module(IO_DECISION_START, IO_DECISION_START+IO_DECISION_SIZE)
{
    // TODO
    decisions=std::vector<CarBrainDecision>(IO_DECISION_SIZE);
}

uint8_t DecisionOutput::read1(const spg_addr_t &address) {
    throw ModuleError("can't read from DecisionOutput");
}

spg_register_t DecisionOutput::read2_be(const spg_addr_t &address) {
    throw ModuleError("can't read from DecisionOutput");
}

void DecisionOutput::write2_be(const spg_addr_t &address, spg_register_t val) {
}

void DecisionOutput::write1(const spg_addr_t &address, uint8_t val) {
}

CarBrainDecision DecisionOutput::popDecision()
{
    //TODO
    return CarBrainDecision::DECISION_FORWARD;
}

bool DecisionOutput::isDecisionAvailable() {
    //TODO
    return false;
}
