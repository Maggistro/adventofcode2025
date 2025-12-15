#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_set>
#include <boost/functional/hash.hpp>

class Machine {


    struct JoltageState {
        int distance;
        int joltageDifference;
        int pressed = 0;
        std::vector<int> joltage;

        void setJoltage(std::vector<int> joltage)
        {
            this->joltage = joltage;
            for (int jolt : joltage) {
                this->joltageDifference += jolt;
            }
        }
    };

    struct JoltageNode
    {
        JoltageState* state;
        JoltageNode* next;
        JoltageNode* previous;
    };

    struct JoltageList {
        JoltageNode* head = nullptr;
        JoltageNode* tail = nullptr;
        int length = 0;

        void insertJoltageState(JoltageState* state)
        {
            JoltageNode* iterator = head;
            JoltageNode* newNode = new JoltageNode{state, nullptr, nullptr};
            if (head == nullptr) 
            {
                head = newNode;
                tail = newNode;
                length++;
                return;
            }

            for (; iterator != nullptr; iterator = iterator->next) 
            {
                if (iterator->state->pressed == state->pressed
                    && iterator->state->joltage == state->joltage) 
                {
                    return;
                } 
            }

            for (iterator = head; iterator != nullptr; iterator = iterator->next) 
            {

                if (iterator->state->distance >= newNode->state->distance) 
                {
                    if (iterator->previous != nullptr) 
                    {
                        iterator->previous->next = newNode;
                        newNode->previous = iterator->previous;
                        iterator->previous = newNode;
                        newNode->next = iterator;
                    } 
                    else 
                    {
                        newNode->next = head;
                        head->previous = newNode;
                        head = newNode;
                    }
                    break;
                }
            }

            if (iterator == nullptr) 
            {
                tail->next = newNode;
                newNode->previous = tail;
                tail = newNode;
            }
            length++;
        }

        void removeHead()
        {
            JoltageNode* oldHead = head;
            if (head->next != nullptr)
            {
                head->next->previous = nullptr;
                head = head->next;
            }
            else
            {
                head = nullptr;
            }
            delete(oldHead->state);
            delete(oldHead);
            length--;
        }

        bool exists(JoltageState* state)
        {
            JoltageNode* iterator = head;
            
            for (; iterator != nullptr; iterator = iterator->next) 
            {
                if (iterator->state->pressed == state->pressed
                    && iterator->state->joltage == state->joltage) 
                {
                    return true;
                } 
            }
            return false;
        }
    };

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
        JoltageState* currentState = new JoltageState();
        currentState->setJoltage(joltageRequirements);
        int totalJoltage = currentState->joltageDifference;
        std::unordered_set<size_t> invalidStates;
        
        JoltageList searchRim;
        searchRim.insertJoltageState(currentState);
        currentState->distance = currentState->joltageDifference;

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
            // create all children
            bool newStateFound = false;
            std::vector<JoltageState*> nextStates;
            for (std::vector<int> positionTransition : positionTransitions) {
                JoltageState* newState = new JoltageState();
                std::vector<int> nextJoltages = std::vector<int>(currentState->joltage);
                int leftOver = 0;

                for (int position : positionTransition) 
                {
                    nextJoltages[position] -= 1;
                    leftOver += nextJoltages[position];
                    if (nextJoltages[position] < 0) {
                        nextJoltages = std::vector<int>{};
                        break;
                    }
                }


                if (!nextJoltages.empty())
                {
                    
                    if (invalidStates.find(boost::hash_range(nextJoltages.begin(), nextJoltages.end())) != invalidStates.end())
                    {
                        delete(newState);
                        continue;
                    }
                    
                    newState->setJoltage(nextJoltages);

                    newState->pressed = currentState->pressed + 1;

                    if (searchRim.exists(newState))
                    {
                        delete(newState);
                        continue;
                    }

                    newStateFound = true;

                    if (newState->joltageDifference == 0) 
                    {
                        return newState->pressed;
                    }
                    
                    
                    int maxLeftPosition = 0;
                    for (int joltage : newState->joltage) 
                    {
                        maxLeftPosition = std::max(maxLeftPosition, joltage);
                    }

                    // newState->distance = (newState->joltageDifference * 100)/totalJoltage + newState->pressed * 10;
                    // newState->distance = newState->pressed;
                    // newState->distance = maxLeftPosition;
                    newState->distance = newState->joltageDifference - leftOver;
                    // newState->distance = -(currentState->joltageDifference - newState->joltageDifference) 
                    //                     + newState->pressed * (newState->joltageDifference  - totalJoltage) / totalJoltage;

                    // std::cout << "\nNew state found with distance " << newState->distance << " and pressed " << newState->pressed << " standing at " << newState->joltageDifference << std::endl;
                    std::cout << "JoltageDifference: " << newState->joltageDifference << std::endl;

                    nextStates.push_back(newState);
                }
            }

            if (!newStateFound)
            {
                invalidStates.insert(boost::hash_range(currentState->joltage.begin(), currentState->joltage.end()));
            }

            searchRim.removeHead();
            if (newStateFound)
            {
                for (JoltageState* nextState : nextStates)
                {
                    searchRim.insertJoltageState(nextState);
                }
            }
            currentState = searchRim.head->state;
        }
    }
};