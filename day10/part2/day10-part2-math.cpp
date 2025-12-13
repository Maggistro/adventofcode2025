#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "../Machine.cpp"
#include <cmath>
#include <map>

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

    (2,3,5,6,8) (0,3,7) (1,4,8) (0,1,6) (3,4,5,8) (2,3,7,8) (0,2,3,8) 

    {21,15,27,149,126,127,16,12,153}

    for (transition: transitions) {
        int maxUsage = 0;
        for (int position = 0; position < joltage.size(); position++) {
            if (transition & (1 << position)) {
                maxUsage = std::min(maxUsage, joltage[position]);
            }
        }
    }



    0 0 1 1 0 1 1 0 1 | 16 , 5   1
    1 0 0 1 0 0 0 1 0 | 12 , 3   2
    0 1 0 0 1 0 0 0 1 | 15 , 3   3
    1 1 0 0 0 0 1 0 0 | 15 , 3   4
    0 0 0 1 1 1 0 0 1 | 126, 4   5
    0 0 1 1 0 0 0 1 1 | 12, 4    6
    1 0 1 1 0 0 0 0 1 | 21, 4    7

    21  27  126 16  153
      15  149 127 12 
    
    => 5

    
    0 0 1 1 0 1 1 0 1 | 1 , 5   1
    1 0 0 1 0 0 0 1 0 | 12 , 3   2
    0 1 0 0 1 0 0 0 1 | 0 , 3   3
    1 1 0 0 0 0 1 0 0 | 15 , 3   4
    0 0 0 1 1 1 0 0 1 | 0, 4   5
    0 0 1 1 0 0 0 1 1 | 12, 4    6
    1 0 1 1 0 0 0 0 1 | 21, 4    7

    21  27  0   16  27
      15  23  1   12 
    
    => 4

    6   27   0  1   27
      0   23  1   12 

    0 0 1 1 0 1 1 0 1 | 1 , 5   1
    1 0 0 1 0 0 0 1 0 | 6 , 3   2
    0 1 0 0 1 0 0 0 1 | 0 , 3   3
    1 1 0 0 0 0 1 0 0 | 0 , 3   4
    0 0 0 1 1 1 0 0 1 | 0, 4   5
    0 0 1 1 0 0 0 1 1 | 23, 4    6
    1 0 1 1 0 0 0 0 1 | 6, 4    7

    // break and go back

    6   27   0  1   27
      0   23  1   12 

    => 6

    6   4   0   1   4
      0   0   1  12


    0 0 1 1 0 1 1 0 1 | 0 , 5   1
    1 0 0 1 0 0 0 1 0 | 0 , 3   2
    0 1 0 0 1 0 0 0 1 | 0 , 3   3
    1 1 0 0 0 0 1 0 0 | 0 , 3   4
    0 0 0 1 1 1 0 0 1 | 0, 4   5
    0 0 1 1 0 0 0 1 1 | 0, 4    6
    1 0 1 1 0 0 0 0 1 | 0, 4    7

    // break and go back


    6   27   0  1   27
      0   23  1   12 

    => 2

    
    0   27  0   1   27
      0   15  1   6 

    0 0 1 1 0 1 1 0 1 | 1 , 5   1
    1 0 0 1 0 0 0 1 0 | 0 , 3   2
    0 1 0 0 1 0 0 0 1 | 0 , 3   3
    1 1 0 0 0 0 1 0 0 | 0 , 3   4
    0 0 0 1 1 1 0 0 1 | 0, 4   5
    0 0 1 1 0 0 0 1 1 | 6, 4    6
    1 0 1 1 0 0 0 0 1 | 0, 4    7

    // break and go back



    long fewestButtonPresses = 0;
    for (Machine* machine : machines) {
        int presses = machine->getFewestJoltageButtonPresses();
        fewestButtonPresses += presses;
        std::cout << "Current fewest button presses: " << presses << std::endl;
    }

    std::cout << "Fewest button presses: " << fewestButtonPresses << std::endl;

    return 0;
}