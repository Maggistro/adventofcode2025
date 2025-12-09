#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <boost/algorithm/string.hpp>
#include <set>
#include <algorithm>

struct Point {
    int x;
    int y;
    int z;

    std::set<Point*> connections;

    float distanceToPoint(Point* other) {
        return std::sqrt(
            std::pow(this->x - other->x, 2) +
            std::pow(this->y - other->y, 2) +
            std::pow(this->z - other->z, 2)
        );
    }

    void addConnection(Point* other) {
        this->connections.insert(other);
    }
};

void generateCircuit(Point* point, std::set<Point*>* circuit)
{
    for (Point* connection : point->connections) {
        if (circuit->find(connection) == circuit->end()) {
            circuit->insert(connection);
            generateCircuit(connection, circuit);
        }
    }
}

std::pair<float, std::vector<Point*>> getShortestConnection(std::vector<Point*> points)
{
    std::pair<float, std::vector<Point*>> connection = std::make_pair(FLT_MAX, std::vector<Point*>{});
    for (Point* firstPoint : points)
    {
        float distance = FLT_MAX;
        Point* closestPoint = nullptr;
        for (Point* secondPoint : points)
        {
            if (firstPoint != secondPoint
                && firstPoint->connections.find(secondPoint) == firstPoint->connections.end()
                && firstPoint->distanceToPoint(secondPoint) < distance)
            {
                distance = firstPoint->distanceToPoint(secondPoint);
                closestPoint = secondPoint;
            }
        }

        if (closestPoint != nullptr)
        {
            if (connection.first > distance)
            {
                connection = std::make_pair(distance, std::vector<Point*>{firstPoint, closestPoint});
            }
        }
    }

    return connection;
}



void buildRim(std::vector<Point*> points)
{
    std::vector<std::vector<Point*>> circuits;
    for (Point* firstPoint: points)
    {
        float distance = FLT_MAX;
        Point* closestPoint = nullptr;
        for (Point* secondPoint : points)
        {
            if (firstPoint != secondPoint
                && firstPoint->connections.find(secondPoint) == firstPoint->connections.end()
                && firstPoint->distanceToPoint(secondPoint) < distance)
            {
                distance = firstPoint->distanceToPoint(secondPoint);
                closestPoint = secondPoint;
            }
        }

        if (closestPoint != nullptr)
        {
            firstPoint->addConnection(closestPoint);
            closestPoint->addConnection(firstPoint);
            circuits.push_back(std::vector<Point*>{firstPoint, closestPoint});
        }
    }

    
}

int main() {
    std::fstream in("../data");
    int subsetSize = 1000;
    std::vector<Point*> points;

    for(std::string line; std::getline(in, line);) {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(","));
        points.push_back(new Point{std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2])});
    }

    std::vector<std::pair<float, std::vector<Point*>>> connections;

    // connect all shortest pairs
    for (int setSize = 0; setSize < subsetSize; setSize++)
    {
        std::pair<float, std::vector<Point*>> connection = getShortestConnection(points);
        connection.second[0]->addConnection(connection.second[1]);
        connection.second[1]->addConnection(connection.second[0]);
        connections.push_back(connection);
    }

    // sort connections by distance
    std::sort(connections.begin(), connections.end(), [](auto a, auto b) {
        return a.first < b.first;
    });

    std::vector<std::pair<float, std::vector<Point*>>> subset = std::vector<std::pair<float, std::vector<Point*>>>(connections.begin(), connections.begin() + subsetSize);

    // create circuits for lowest distance pairs
    std::set<std::set<Point*>> circuits;
    std::vector<int> circuitSizes;
    for (std::pair<float, std::vector<Point*>> connection : subset)
    {
        std::set<Point*> circuit;
        generateCircuit(connection.second[0], &circuit);
        circuits.insert(circuit);
    }

    for(std::set<Point*> circuit : circuits)
    {
        circuitSizes.push_back(circuit.size());
    }

    // sort cuircuit sizes
    std::sort(circuitSizes.begin(), circuitSizes.end());

    std::cout << "3 biggest number of circuits multiplied: " << circuitSizes[circuitSizes.size() - 1] * circuitSizes[circuitSizes.size() - 2] * circuitSizes[circuitSizes.size() - 3] << std::endl;

    return 0;
}