#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../Machine.cpp"
#include <cmath>

int main() {
    std::fstream in("../data");

    std::vector<std::string> parts;
    std::vector<Machine*> machines;
    for (std::string line; std::getline(in, line);) {
        boost::split(parts, line, boost::is_any_of(" "));

        // get target state as mask
        boost::replace_all(parts[0], ".", "0");
        boost::replace_all(parts[0], "#", "1");
        std::string mask = std::string{parts[0].begin() + 1, parts[0].end() - 1};
        std::reverse(mask.begin(), mask.end());
        uint16_t targetState = (uint16_t)stol(mask, nullptr, 2);

        // get transitions
        std::vector<uint16_t> transitions;
        for(std::string transition : std::vector<std::string>{parts.begin() + 1, parts.end() - 1})
        {
            std::vector<std::string> transitionParts;
            boost::split(transitionParts, std::string{transition.begin() + 1, transition.end() - 1}, boost::is_any_of(","));

            
            uint16_t transitionMask = 0;
            for(std::string fromTo : transitionParts)
            {
                transitionMask += (uint16_t)(std::pow(2 ,std::stoi(fromTo)));
            }
            transitions.push_back(transitionMask);
        }

        // get joltage requirements
        std::vector<std::string> joltageParts;
        std::vector<int> joltageRequirements;
        boost::split(joltageParts, std::string{parts[parts.size() - 1].begin() + 1, parts[parts.size() - 1].end() - 1}, boost::is_any_of(","));
        for(std::string joltageStr : joltageParts)
        {
            joltageRequirements.push_back(stoi(joltageStr));
        }

        Machine* machine = new Machine(
            targetState,
            transitions,
            joltageRequirements
        );
        machines.push_back(machine);
    }

    long fewestButtonPresses = 0;
    for (Machine* machine : machines) {
        fewestButtonPresses += machine->getFewestButtonPresses();
    }

    std::cout << "Fewest button presses: " << fewestButtonPresses << std::endl;

    return 0;
}