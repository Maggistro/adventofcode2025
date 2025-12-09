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
};

std::pair<int, int> buildArea(std::pair<int, int> currentPosition, std::vector<std::pair<int, int>>& cornerPoints, std::vector<std::pair<int, int>>& points)
{
    std::set<std::pair<int, int>> rimPoints;
    int distance = INT_MAX;

    std::pair<int, int> nextPoint = std::make_pair(0,0);
    std::pair<int, int> lastPoint = *(cornerPoints.end() - 2);
    for(auto& point : points) {
        if (point == lastPoint || point == currentPosition) {
            continue;
        }

        // Optimize
        if (point.first == currentPosition.first) {
            int distanceY = std::abs(currentPosition.second - point.second);
            if (distanceY < distance) {
                distance = distanceY;
                nextPoint = point;
            }
        }

        if (point.second == currentPosition.second) {
            int distanceX = std::abs(currentPosition.first - point.first);
            if (distanceX < distance) {
                distance = distanceX;
                nextPoint = point;
            }
        }
    }

    cornerPoints.push_back(nextPoint);

    return nextPoint;
}

bool isFilled(std::pair<int, int> firstPoint, std::pair<int, int> secondPoint, std::vector<std::pair<int, int>>& cornerPoints)
{
    PreviousPosition prevPos;
    for (auto& point : cornerPoints) {
        if (point == firstPoint || point == secondPoint) {
            prevPos.xRight = false;
            prevPos.yTop = false;
            prevPos.xLeft = false;
            prevPos.yBottom = false;
            continue;
        }

        // completely inside
        if (point.first > std::min(firstPoint.first, secondPoint.first)
            && point.first < std::max(firstPoint.first, secondPoint.first)
            && point.second > std::min(firstPoint.second, secondPoint.second)
            && point.second < std::max(firstPoint.second, secondPoint.second)) {
            return false;
        }

        // previously on rim, move out
        if (prevPos.xRight || prevPos.xLeft || prevPos.yTop || prevPos.yBottom)
        {
            if (prevPos.xRight) {
                if (point.first < std::max(firstPoint.first, secondPoint.first)) {
                    return false;
                }
            }

            if (prevPos.xLeft) {
                if (point.first > std::min(firstPoint.first, secondPoint.first)) {
                    return false;
                }
            }

            if (prevPos.yTop) {
                if (point.second < std::max(firstPoint.second, secondPoint.second)) {
                    return false;
                }
            }

            if (prevPos.yBottom) {
                if (point.second > std::min(firstPoint.second, secondPoint.second)) {
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

    cornerPoints.push_back(minPoint);
    // initial start direction in x
    int distanceX = INT_MAX;

    std::pair<int, int> nextPoint = std::make_pair(0,0);
    for(auto& point : points) {
        if (point == minPoint) {
            continue;
        }

        if (point.second == minPoint.second) {
            int distance = point.first - minPoint.first;
            if (distance > 0 && distance < distanceX) {
                distanceX = distance;
                nextPoint = point;
            }
        }
    }

    cornerPoints.push_back(nextPoint);
    
    // get outer path
    do
    {
        nextPoint = buildArea(nextPoint, cornerPoints, points);
    } while (nextPoint != minPoint);


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
