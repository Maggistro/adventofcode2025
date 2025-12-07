#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

struct Beam {
    int position;
    long long timelineCount = 0;

    Beam* parentBeam = nullptr;
    Beam* leftBeam;
    Beam* rightBeam;
};

long long addTimelineCount(Beam* beam) {
    
    if (beam->leftBeam != nullptr) {
        if (beam->leftBeam->timelineCount != 0) {
            beam->timelineCount += beam->leftBeam->timelineCount;
        }
        else
        {
            beam->timelineCount += addTimelineCount(beam->leftBeam);
        }
    }

    if (beam->rightBeam != nullptr) {
        if (beam->rightBeam->timelineCount != 0) {
            beam->timelineCount += beam->rightBeam->timelineCount;
        }
        else
        {
            beam->timelineCount += addTimelineCount(beam->rightBeam);
        }
    }
    
    std::cout << "Beam at position " << beam->position << " has " << beam->timelineCount << " timelines." << std::endl;

    return beam->timelineCount == 0 ? 1 : beam->timelineCount;
}

int main() {
    std::fstream in("../data");
    std::string line;
    Beam InitialBeam;
    std::map<int, Beam*> currentBeams;
    int lineNumber = 0;

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
        lineNumber++;

        std::map<int, Beam*> nextBeams;
        for(auto pair : currentBeams) {
            Beam* beam = pair.second;
            if (line[beam->position] == '^')
            {
                if (nextBeams.find(beam->position - 1) != nextBeams.end())
                {
                    // already exists, just link
                    beam->leftBeam = nextBeams[beam->position - 1];                        
                }
                else
                {
                    Beam* left = new Beam();
                    left->position = beam->position - 1;
                    left->parentBeam = beam;
                    beam->leftBeam = left;
                    nextBeams.insert(std::make_pair(left->position, left));  
                }

                if (nextBeams.find(beam->position + 1) != nextBeams.end())
                {
                    // already exists, just link
                    beam->rightBeam = nextBeams[beam->position + 1];                        
                }
                else
                {
                    Beam* right = new Beam();
                    right->position = beam->position + 1;
                    right->parentBeam = beam;
                    beam->rightBeam = right;
                    nextBeams.insert(std::make_pair(right->position, right));
                }
            }
            else
            {
                if (nextBeams.find(beam->position) != nextBeams.end())
                {
                    // already exists, replace it
                    Beam* existing =  nextBeams[beam->position];
                    if (existing->parentBeam != nullptr)
                    {
                        if (existing->parentBeam->leftBeam->position == beam->position)
                        {
                            existing->parentBeam->leftBeam = beam;
                        }
                        else
                        {
                            existing->parentBeam->rightBeam = beam;
                        }
                    }
                    nextBeams.erase(beam->position);
                    nextBeams.insert(std::make_pair(beam->position, beam));
                }
                else
                {
                    nextBeams.insert(std::make_pair(beam->position, beam));
                }
            }
        }
        currentBeams = nextBeams;
        std::cout << "Processing line: " << lineNumber * 2 << " with Beams " << currentBeams.size() << std::endl;
    }

    addTimelineCount(&InitialBeam);

    std::cout << "Total timelines: " << InitialBeam.timelineCount << std::endl;
}