#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <set>

#include "../Server.cpp"

int traverseConnection(Server* currentServer, std::map<std::string, Server*>* servers) {
    
    int pathCount = 0;
    for (std::string connection : currentServer->getConnections()) {
        if (connection == "out") {
            pathCount++;
            continue;
        }

        if (connection == "you") {
            continue;
        }

        pathCount += traverseConnection(servers->at(connection), servers);
    }

    return pathCount;
}

int main() {
    std::fstream in("../data");
    std::map<std::string, Server*> servers;
    std::set<std::vector<std::string>> paths;

    for (std::string line; std::getline(in, line);) {
        std::vector<std::string> parts;
        boost::split(parts, line, boost::is_any_of(" "));
        Server* server = new Server(
            parts[0].substr(0, parts[0].size() - 1),
            std::vector<std::string>(parts.begin() + 1, parts.end())
        );

        servers.insert({parts.begin()->substr(0, parts[0].size() - 1), server});
    }


    Server* currentServer = servers.at("you");

    std::cout << "Total paths: " << traverseConnection(currentServer, &servers) << std::endl;
}