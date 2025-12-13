#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>

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

    long getFewestJoltageButtonPresses() {
        std::vector<std::vector<int>> currentStates = {joltageRequirements};
        std::vector<std::vector<int>> positionTransitions;
        long presses = 1;


        for (int i = 0; i < transitions.size(); i++)
        {
            int position = 0;
            std::vector<int> positionTransition;
            for (int bit = 0; bit < 16; bit++) {
                position = ((1 << bit) & transitions[i]) > 0 ? 1 : 0;
                if (position > 0) {
                    positionTransition.push_back(bit);
                    position = 0;
                }
            }
            positionTransitions.push_back(positionTransition);
        }

        while(true) // assuming we get there eventually
        {
            std::vector<std::vector<int>> nextStates;
            for (std::vector<int> state : currentStates) {
                for (std::vector<int> positionTransition : positionTransitions) {
                    std::vector<int> nextState = state;
                    for (int position : positionTransition) {
                        nextState[position] -= 1;
                        if (nextState[position] < 0) {
                            nextState = std::vector<int>{};
                            break;
                        }
                    }
                    if (!nextState.empty() && std::all_of(nextState.begin(), nextState.end(), [](int i) { return i==0; })) {
                        return presses;
                    }   
                    if (!nextState.empty())
                    {
                        nextStates.push_back(nextState);                           
                    }
                }
            }
            currentStates = nextStates;
            presses++;
        }
    }
};