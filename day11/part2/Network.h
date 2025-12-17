#ifndef NETWORK_H
#define NETWORK_H

#include <set>
#include <string>
#include <utility>
#include <map>
#include "../Server.h"

class Network {
private:
    std::map<std::pair<std::string, std::string>, int> connections = std::map<std::pair<std::string, std::string>, int>{};
    std::map<std::pair<std::string, std::string>, int> connectionSplits = std::map<std::pair<std::string, std::string>, int>{};

public:
    Network() = default;
    
    void step(std::map<std::string, Server*>& servers, std::string stepTarget);
    
    std::map<std::pair<std::string, std::string>, int>* getConnections();

    void setStartServer(const std::string serverName);

    void insertConnection(std::pair<std::string, std::string> connection, std::map<std::pair<std::string, std::string>, int>* map = nullptr, int visitCount = 1);
    void removeConnection(std::pair<std::string, std::string> connection, std::map<std::pair<std::string, std::string>, int>* map = nullptr, int visitCount = 1);

    void mergeConnections();

    unsigned long long findAllWithTarget(std::string target);
};

#endif // NETWORK_H
