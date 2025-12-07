#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

int main() {
    std::fstream in("../data");
    std::set<int> beams;
    std::string line;
    int splitCount = 0;

    // get initial beam
    std::getline(in, line);
    for (int pos = 0; pos < line.size(); pos++) {
        if (line[pos] == 'S') {
            beams.insert(pos);
            break;
        }
    }

    // check each second line for splitter
    for (line; std::getline(in, line); ) {
        std::getline(in, line);

        std::set<int> currentBeams = beams; // copy current beams to iterate over
        for(int beam : currentBeams) {
           if (line[beam] == '^')
           {
                beams.erase(beam);
                beams.insert(beam - 1);
                beams.insert(beam + 1);
                splitCount++;
           }
        }
    }

    std::cout << "Total splits: " << splitCount << std::endl;

    return 0;
}