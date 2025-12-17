#include "Network.h"
#include <tuple>

void Network::step(std::map<std::string, Server*>& servers, std::string stepTarget) {
    std::map<std::pair<std::string, std::string>, int> newConnections = std::map<std::pair<std::string, std::string>, int>{};
    for (const auto& [connection, visitCount] : connections) {
        const auto& [node1, node2] = connection;    
        if (node2 == "out") 
        {
            insertConnection({node1, node2}, &newConnections, visitCount);
            continue;
        }
        if (node2 == stepTarget) 
        {
            insertConnection({node1, node2}, &newConnections, visitCount);
            continue;
        }

        Server* server = servers.at(node2);

        // simplify path if only 1 connection
        if (server->getConnections().size() == 1) {
            removeConnection({node1, node2}, &newConnections, 1);
            insertConnection({node1, server->getConnections()[0]}, &newConnections, visitCount);
            continue;
        }

        if (node1 != node2)
        {
            insertConnection({node1, server->getName()}, &connectionSplits, visitCount);
        }
        // start new path if more than 1 connection
        for (const std::string& connection : server->getConnections()) {
            insertConnection({server->getName(), connection}, &newConnections, visitCount);
        }
    }
    connections = newConnections;
}

void Network::setStartServer(const std::string node1) {
    insertConnection({node1, node1}, &connections);
}

std::map<std::pair<std::string, std::string>, int>* Network::getConnections() {
    return &connections;
}

void Network::mergeConnections() {
    connections.insert(connectionSplits.begin(), connectionSplits.end());
}

void Network::removeConnection(std::pair<std::string, std::string> connection, std::map<std::pair<std::string, std::string>, int>* map, int visitCount) {
    if (map->find(connection) != map->end()) {
        if (map->at(connection) > 1) {
            map->at(connection) -= visitCount;
        } else {
            map->erase(connection);
        }
    }
}

void Network::insertConnection(std::pair<std::string, std::string> connection, std::map<std::pair<std::string, std::string>, int>* map, int visitCount) {
    if (map->find(connection) != map->end()) {
        map->at(connection) += visitCount;
        return;
    }
    map->insert({connection, visitCount});
}

unsigned long long Network::findAllWithTarget(std::string target) {
    unsigned long long count = 0;
    for (const auto& [connection, visitCount] : connections) {
        const auto& [node1, node2] = connection;
        if (node2 == target) {
            count += visitCount;
        }
    }
    return count;
}