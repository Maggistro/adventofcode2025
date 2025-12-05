#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

struct Range {
    long long start;
    long long end;

    bool isInsideRange(long long value) const {
        return (value >= start) && (value <= end);
    }

    Range(long long s, long long e) : start(s), end(e) 
    {
    }
};

int main() {
    std::fstream in("../data");

    std::vector<Range> fresh;

    std::string line;

    int freshIngridientCount = 0;

    // get ranges
    while (std::getline(in, line) && !line.empty())
    {
        fresh.push_back(
            Range(
                std::stoll(line.substr(0, line.find('-'))),
                std::stoll(line.substr(line.find('-') + 1))
            )
        );    
    }
    
    // check ingridients
    while (std::getline(in, line))
    {
        for(Range range : fresh) {
            long long value = std::stoll(line);
            if (range.isInsideRange(value)) {
                // std::cout << "Value " << value << " is inside range " << range.start << "-" << range.end << std::endl;
                freshIngridientCount++;
                break;
            }
        }
    }

    std::cout << "Fresh ingredient count: " << freshIngridientCount << std::endl;
}