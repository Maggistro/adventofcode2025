#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

bool isFilled(std::pair<int, int> firstPoint, std::pair<int, int> secondPoint, std::vector<std::pair<int, int>>& cornerPoints)
{
    std::pair<int, int> lastPoint = cornerPoints.end()[-1];

    for (auto& point : cornerPoints) {

        // is inside
        if (point.first > std::min(firstPoint.first, secondPoint.first)
            && point.first < std::max(firstPoint.first, secondPoint.first)
            && point.second > std::min(firstPoint.second, secondPoint.second)
            && point.second < std::max(firstPoint.second, secondPoint.second)) {
            return false;
        }

        // passed through on y axis
        if (point.first > std::min(firstPoint.first, secondPoint.first)
            && point.first < std::max(firstPoint.first, secondPoint.first))
        {
            if ((lastPoint.second <= std::min(firstPoint.second, secondPoint.second)
            && point.second >= std::max(firstPoint.second, secondPoint.second))
            || (point.second <= std::min(firstPoint.second, secondPoint.second)
            && lastPoint.second >= std::max(firstPoint.second, secondPoint.second))) {
                return false;
            }
        }

        // passed through on x axis
        if (point.second > std::min(firstPoint.second, secondPoint.second)
            && point.second < std::max(firstPoint.second, secondPoint.second))
        {
            if ((lastPoint.first <= std::min(firstPoint.first, secondPoint.first)
            && point.first >= std::max(firstPoint.first, secondPoint.first))
            || (point.first <= std::min(firstPoint.first, secondPoint.first)
            && lastPoint.first >= std::max(firstPoint.first, secondPoint.first))) {
                return false;
            }
        }

        lastPoint = point;
    }
    return true;
}

int main() {

    std::ifstream in("../data");
    std::vector<std::pair<int, int>> points;
    std::vector<std::pair<int, int>> cornerPoints;

    std::pair<int, int> minPoint = std::make_pair(INT_MAX, INT_MAX);
    for(std::string line; std::getline(in, line);) {
        std::pair<int, int> newPoint = std::make_pair(std::stoi(line.substr(0, line.find(","))), std::stoi(line.substr(line.find(",") + 1)));

        if (newPoint.first <= minPoint.first && newPoint.second <= minPoint.second) {
            minPoint = newPoint;
        }
        points.push_back(std::make_pair(std::stoi(line.substr(0, line.find(","))), std::stoi(line.substr(line.find(",") + 1))));
    }


    cornerPoints = points;
    cornerPoints.push_back(cornerPoints[0]);


    std::pair<std::pair<int, int>, std::pair<int, int>> bestPoint = std::make_pair(std::make_pair(0,0), std::make_pair(0,0));
    long long area = 0;
    for (int startingPoint = 0; startingPoint < cornerPoints.size(); startingPoint++) {
        std::pair<int, int> firstPoint = cornerPoints[startingPoint];

        for (int endPoint = startingPoint + 1; endPoint < cornerPoints.size(); endPoint++) {
           std::pair<int, int> secondPoint = cornerPoints[endPoint];


            long long newArea = (long long)(std::abs(secondPoint.first - firstPoint.first) + 1) * (long long)(std::abs(secondPoint.second - firstPoint.second) + 1);
            if (newArea > area) {
                if (isFilled(firstPoint, secondPoint, cornerPoints)) {
                    area = newArea;

                    bestPoint = std::make_pair(firstPoint, secondPoint);                
                }
            }
        }
    }
    
    std::cout << "Rectangle area: " << area << std::endl;

    return 0;
}

// 4629483216
// 4629483216
// 4600044000
// 186567246
// 1704850740
// 157191288
// 1525991432