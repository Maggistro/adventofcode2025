#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {

    std::ifstream in("../data");
    std::vector<std::pair<long, long>> points;

    for(std::string line; std::getline(in, line);) {
        points.push_back(std::make_pair(std::stol(line.substr(0, line.find(","))), std::stol(line.substr(line.find(",") + 1))));
    }

    std::pair<std::pair<long, long>, std::pair<long, long>> bestPoint = std::make_pair(std::make_pair(0,0), std::make_pair(0,0));
    long long area = 0;
    for (auto& firstPoint : points) {
        for (auto& secondPoint : points) {
            if (firstPoint == secondPoint) {
                continue;
            }

            long long newArea = (long long)(std::abs(secondPoint.first - firstPoint.first) + 1) * (long long)(std::abs(secondPoint.second - firstPoint.second) + 1);
            if (newArea > area) {
                area = newArea;

                bestPoint = std::make_pair(firstPoint, secondPoint);
            }
        }
    }
    
    std::cout << "Rectangle area: " << area << std::endl;

    return 0;
}
// 4748769124
// 2147427810
// 453801828