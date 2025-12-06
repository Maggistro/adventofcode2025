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

    bool overlapsWith(const Range& other) const {
        return (other.start <= start && other.end >= start) // overlap at the start
            || (other.start <= end && other.end >= end) // overlap at the end
            || (other.start >= start && other.end <= end) // inside range
            || (other.start <= start && other.end >= end); // enclosing range
    }

    void merge(const Range& other) {
        start = std::min(start, other.start);
        end = std::max(end, other.end);
    }

    Range(long long s, long long e) : start(s), end(e) 
    {
    }
};

void updateRanges(std::vector<Range>& ranges, long long index) {
    bool merged = false;

    for (long long i = 0; i < index; ++i) {
        if (ranges[i].overlapsWith(ranges[index])) {
            ranges[i].merge(ranges[index]);
            merged = true;
            break;
        }
    }

    if (merged) {
        ranges.erase(ranges.begin() + index);
    }

    if (index > 0)
    {
        updateRanges(ranges, index - 1);
    }
}

int main() {
    std::fstream in("../data");

    std::vector<Range> fresh;

    std::string line;

    long long freshIngridientCount = 0;
    bool overlapped = false;
    
    // get ranges and do a basic merge
    while (std::getline(in, line) && !line.empty())
    {
        overlapped = false;
        Range newRange(
            std::stoll(line.substr(0, line.find('-'))),
            std::stoll(line.substr(line.find('-') + 1))
        );
        
        for(Range& range : fresh) {
            if (range.overlapsWith(newRange)) {
                range.merge(newRange);
                overlapped = true;
                break;
            }
        }

        if (!overlapped) {
            fresh.push_back(newRange);
        }
    }

    // merge leftover ranges inside list
    updateRanges(fresh, fresh.size() - 1);

    // count fresh ingredients
    for (const Range& range : fresh) {
        freshIngridientCount += (range.end - range.start + 1);
    }
    
    std::cout << "Fresh ingredient count: " << freshIngridientCount << std::endl;
}

// 360295847145439

// 362982296500358 to high