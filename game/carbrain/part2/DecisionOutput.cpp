#include "DecisionOutput.h"

DecisionOutput::DecisionOutput(const spg_addr_t start, uint8_t max_decisions) : Module(start, 1)
{
    // TODO
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




void DecisionOutput::write1(const spg_addr_t &address, uint8_t val) {
    //TODO Implementeer de virtuele methoden van BotModule, om 1 of 2 bytes te schrijven naar de lijst beslissingen
    //TODO Implementeer de virtuele methoden van BotModule ivm lezen door een exceptie te gooien
}
