#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

struct Beam {
    int position;
    int timelineCount = 0;

    Beam* parentBeam = nullptr;
    Beam* leftBeam;
    Beam* rightBeam;
};

long long addTimelineCount(Beam* beam) {
    
    if (beam->leftBeam != nullptr) {
        beam->timelineCount += addTimelineCount(beam->leftBeam);
    }

    if (beam->rightBeam != nullptr) {
        beam->timelineCount += addTimelineCount(beam->rightBeam);
    }
    
    return beam->timelineCount == 0 ? 1 : beam->timelineCount;
}

int main() {
    std::fstream in("../test");
    std::string line;
    Beam InitialBeam;
    std::map<int, Beam*> currentBeams;

    // get initial beam
    std::getline(in, line);
    for (int pos = 0; pos < line.size(); pos++) {
        if (line[pos] == 'S') {
            InitialBeam = Beam{pos};
            currentBeams.insert(std::make_pair(InitialBeam.position, &InitialBeam));
            break;
        }
    }

    // build graph of beams
    for (line; std::getline(in, line); ) {
        if(!std::getline(in, line)) {
            break;
        }

        std::map<int, Beam*> nextBeams;
        for(auto& pair : currentBeams) {
            Beam* beam = pair.second;
            if (line[beam->position] == '^')
            {       
                    Beam* left = new Beam();
                    left->position = beam->position - 1;
                    left->parentBeam = beam;

                    Beam* right = new Beam();
                    right->position = beam->position + 1;
                    right->parentBeam = beam;

                    beam->leftBeam = left;
                    beam->rightBeam = right;
                    nextBeams.insert(std::make_pair(left->position, left));  
                    nextBeams.insert(std::make_pair(right->position, right));
            }
            else 
            {
                nextBeams.insert(std::make_pair(beam->position, beam));
            }
        }
        currentBeams = nextBeams;
    }

    for(auto& pair : currentBeams) {
        Beam* beam = pair.second;
        beam->timelineCount = 1;
    }

    addTimelineCount(&InitialBeam);

    // std::set<Beam*> finalBeams = std::set<Beam*>();
    // for (auto pair : currentBeams)
    // {
    //     finalBeams.insert(pair.second);
    // }
    // // reverse traverse to find all unique timelines
    // while(!finalBeams.empty()) {
    //     std::set<Beam*> parentBeams;

    //     for(Beam* beam : finalBeams) {
    //         if (beam->parentBeam != nullptr) {

    //             beam->parentBeam->timelineCount += beam->timelineCount == 0 ? 1 : beam->timelineCount;
    //             parentBeams.insert(beam->parentBeam);
    //         }
    //     }

    //     finalBeams = parentBeams;
    // }


    std::cout << "Total timelines: " << InitialBeam.timelineCount << std::endl;
}