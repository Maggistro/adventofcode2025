#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <boost/algorithm/string.hpp>
#include <set>
#include <map>
#include <algorithm>

struct Circuit;

struct Point {
    int x;
    int y;
    int z;

    Circuit* circuit;

    float distanceToPoint(Point* other) {
        return std::sqrt(
            std::pow(this->x - other->x, 2) +
            std::pow(this->y - other->y, 2) +
            std::pow(this->z - other->z, 2)
        );
    }

    std::string toString() {
        return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
    }
};

struct Circuit {
    std::set<Point*> points;
};

std::pair<Point*, Point*> getShortestConnection(std::set<Circuit*>* circuits)
{
    std::pair<Point*, Point*> connection = std::make_pair(nullptr, nullptr);
    std::pair<Circuit*, Circuit*> circuitsToMerge = std::make_pair(nullptr, nullptr);

    float distance = FLT_MAX;
    for (auto firstCircuit = circuits->begin(); firstCircuit != circuits->end(); ++firstCircuit)
    {
        std::set<Circuit*>::iterator secondCircuit = std::set<Circuit*>::iterator(firstCircuit);
        secondCircuit++;
        for (; secondCircuit != circuits->end(); ++secondCircuit)
        {
            for (Point* firstPoint : (*firstCircuit)->points)
            {
                for (Point* secondPoint : (*secondCircuit)->points)
                {
                    if(firstPoint->distanceToPoint(secondPoint) < distance)
                    {
                        circuitsToMerge = std::make_pair(*firstCircuit, *secondCircuit);
                        connection = std::make_pair(firstPoint, secondPoint);
                        distance = firstPoint->distanceToPoint(secondPoint);
                    }
                }
            }
        }
    }
    
    Circuit* newCirctuit = new Circuit();
    newCirctuit->points.insert(circuitsToMerge.first->points.begin(), circuitsToMerge.first->points.end());
    newCirctuit->points.insert(circuitsToMerge.second->points.begin(), circuitsToMerge.second->points.end());
    
    circuits->erase(circuitsToMerge.first);
    circuits->erase(circuitsToMerge.second);
    circuits->insert(newCirctuit);

    return connection;
}

int main() {
    std::fstream in("../data");
    std::vector<Point*> points;
    std::set<Circuit*> circuits;

    for(std::string line; std::getline(in, line);) 
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(","));
        points.push_back(new Point{std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2])});
    }

    for (Point* point : points)
    {
        Circuit* circuit = new Circuit();
        circuit->points.insert(point);
        circuits.insert(circuit);
    }

    std::cout << "Initial circuits: " << circuits.size() << std::endl;
    int iteration = 0;
    int lastCount = circuits.size();
    while (circuits.size() > 2)
    {
        getShortestConnection(&circuits);
        if (lastCount != circuits.size())
        {
            lastCount = circuits.size();
            std::cout << "Circuits after iteration " << iteration << ": " << circuits.size() << std::endl;
        }
    }

    std::pair<Point*, Point*> lastConnection = getShortestConnection(&circuits);

    std::cout << "Last connection points: " << lastConnection.first->toString() << " - " << lastConnection.second->toString() << std::endl;

    return 0;
}