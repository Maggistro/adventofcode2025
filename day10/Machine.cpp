#include <vector>
#include <cstdint>

class Machine {

private:
    uint16_t targetStateMask;
    std::vector<uint16_t> transitions; // from_state, to_state
    std::vector<int> joltageRequirements;

public:
    Machine() : targetStateMask(0) {}

    Machine(uint16_t targetMask,
            const std::vector<uint16_t>& trans,
            const std::vector<int>& joltages)
        : targetStateMask(targetMask), 
          transitions(trans), joltageRequirements(joltages) {}

    void setTargetStateMask(uint64_t mask) { targetStateMask = mask; }
    uint64_t getTargetStateMask() const { return targetStateMask; }


    void addTransition(uint16_t from, uint16_t to) { transitions.push_back(from); transitions.push_back(to); }
    const std::vector<uint16_t>& getTransitions() const { return transitions; }

    void addJoltageRequirement(int joltage) { joltageRequirements.push_back(joltage); }
    const std::vector<int>& getJoltageRequirements() const { return joltageRequirements; }

    long getFewestButtonPresses() {

        std::vector<uint16_t> currentStates = {0};
        long presses = 1;
        while(true) // assuming we get there eventually
        {
            std::vector<uint16_t> nextStates;
            for (uint16_t state : currentStates) {
                for (uint16_t transition : transitions) {
                    nextStates.push_back(state ^ transition);      
                    if ((state ^ transition) == targetStateMask) {
                        return presses;
                    }
                }
            }
            currentStates = nextStates;
            presses++;
        }
    }
};