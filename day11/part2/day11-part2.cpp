#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>  
#include <set>

#include "../Server.h"
#include "Network.h"

int main() {
    std::fstream in("../data");
    std::map<std::string, Server*> servers;
    std::set<std::vector<std::string>> paths;

    // freopen("../debug", "w", stdout);

    for (std::string line; std::getline(in, line);) 
    {
        std::vector<std::string> parts;
        boost::split(parts, line, boost::is_any_of(" "));
        Server* server = new Server(
            parts[0].substr(0, parts[0].size() - 1),
            std::vector<std::string>(parts.begin() + 1, parts.end())
        );

        servers.insert({parts.begin()->substr(0, parts[0].size() - 1), server});
    }


    Server* currentServer = servers.at("svr");

    Network* network = new Network();
    network->setStartServer("svr");
    for (int step = 0; step < 25; step++) {
        network->step(servers, "fft");
    }
    network->mergeConnections();
    unsigned long long countToFft = network->findAllWithTarget("fft");

    network = new Network();
    network->setStartServer("fft");
    for (int step = 0; step < 30; step++) {
        network->step(servers, "dac");
    }
    network->mergeConnections();
    unsigned long long countToDac = network->findAllWithTarget("dac");

    network = new Network();
    network->setStartServer("dac");
    for (int step = 0; step < 25; step++) {
        network->step(servers, "out");
    }
    network->mergeConnections();
    unsigned long long countToOut = network->findAllWithTarget("out");

    std::cout << "Total paths to fft: " << countToFft * countToDac * countToOut << std::endl;
}