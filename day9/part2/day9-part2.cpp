#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>


struct PreviousPosition
{
    bool xLeft = false;
    bool yBottom = false;
    bool xRight = false;
    bool yTop = false;
    std::pair<int, int> lastPoint;
};

bool isFilled(std::pair<int, int> firstPoint, std::pair<int, int> secondPoint, std::vector<std::pair<int, int>>& cornerPoints)
{
    PreviousPosition prevPos;
    prevPos.lastPoint = cornerPoints.end()[-1];

    // prevPos.xLeft = firstPoint.first < secondPoint.first;
    // prevPos.xRight = firstPoint.first > secondPoint.first;
    // prevPos.yBottom = firstPoint.second < secondPoint.second;
    // prevPos.yTop = firstPoint.second > secondPoint.second;

    for (auto& point : cornerPoints) {

        // completely inside
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
            if ((prevPos.lastPoint.second <= std::min(firstPoint.second, secondPoint.second)
            && point.second >= std::max(firstPoint.second, secondPoint.second))
            || (point.second <= std::min(firstPoint.second, secondPoint.second)
            && prevPos.lastPoint.second >= std::max(firstPoint.second, secondPoint.second))) {
                return false;
            }
        }

        // passed through on x axis
        if (point.second > std::min(firstPoint.second, secondPoint.second)
            && point.second < std::max(firstPoint.second, secondPoint.second))
        {
            if ((prevPos.lastPoint.first <= std::min(firstPoint.first, secondPoint.first)
            && point.first >= std::max(firstPoint.first, secondPoint.first))
            || (point.first <= std::min(firstPoint.first, secondPoint.first)
            && prevPos.lastPoint.first >= std::max(firstPoint.first, secondPoint.first))) {
                return false;
            }
        }


        // previously on rim, move out
        if (prevPos.xRight || prevPos.xLeft || prevPos.yTop || prevPos.yBottom)
        {
            if (prevPos.xRight) {
                if (point.first < std::max(firstPoint.first, secondPoint.first) && !(prevPos.yTop || prevPos.yBottom)) {
                    return false;
                }
            }

            if (prevPos.xLeft) {
                if (point.first > std::min(firstPoint.first, secondPoint.first) && !(prevPos.yTop || prevPos.yBottom)) {
                    return false;
                }
            }

            if (prevPos.yTop) {
                if (point.second < std::max(firstPoint.second, secondPoint.second) && !(prevPos.xLeft || prevPos.xRight)) {
                    return false;
                }
            }

            if (prevPos.yBottom) {
                if (point.second > std::min(firstPoint.second, secondPoint.second) && !(prevPos.xLeft || prevPos.xRight)) {
                    return false;
                }
            }
        }

        // on rim, next point only allowed to move out
        if (
            (point.first == std::min(firstPoint.first, secondPoint.first) // point has same x as left side
            || point.first == std::max(firstPoint.first, secondPoint.first))  // or has same x as right side
            && (point.second >= std::min(firstPoint.second, secondPoint.second) // and is between top and bottom
            && point.second <=  std::max(firstPoint.second, secondPoint.second))

            || (point.second == std::min(firstPoint.second, secondPoint.second) // point has same y as bottom side
            || point.second ==  std::max(firstPoint.second, secondPoint.second)) // or has same y as top side
            && (point.first >= std::min(firstPoint.first, secondPoint.first) // and is is between left and right
            && point.first <= std::max(firstPoint.first, secondPoint.first))
        )
        {
            prevPos.xLeft = point.first == std::min(firstPoint.first, secondPoint.first);
            prevPos.yBottom = point.second == std::min(firstPoint.second, secondPoint.second);
            prevPos.xRight = point.first == std::max(firstPoint.first, secondPoint.first);
            prevPos.yTop = point.second ==  std::max(firstPoint.second, secondPoint.second);
            continue;
        }

        prevPos.xRight = false;
        prevPos.yTop = false;
        prevPos.xLeft = false;
        prevPos.yBottom = false;
        prevPos.lastPoint = point;

        // outside x range
        // outside y range
        // in x range, but next not in y range
        // in y range, but next not in x range
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
// 157191288