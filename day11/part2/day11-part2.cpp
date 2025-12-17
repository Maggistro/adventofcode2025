#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/multiprecision/cpp_int.hpp>  
#include <set>

#include "../Server.h"
#include "Network.h"

boost::multiprecision::int128_t findAllConnectionsBetween(std::string start, std::string target, Network* network, std::pair<std::string, std::string> lastNode = std::pair<std::string, std::string>("","")) {
    if (start == target) {
        // network->removeConnection(lastNode);
        return 1;
    }

    if (target == "svr") {
        // network->removeConnection(lastNode);
        return 0;
    }

    boost::multiprecision::int128_t count = 0;

    for (const auto& [connection, visitCount] : *network->getConnections()) {
        const auto& [node1, node2] = connection;
        if (node2 == target) {
            count += visitCount * findAllConnectionsBetween(start, node1, network, std::pair<std::string, std::string>(node1, node2));
        }
    }

    return count;
}

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

    // // find paths from svr to fft
    boost::multiprecision::int128_t  pathsToFft = findAllConnectionsBetween("svr", "fft", network);
    std::cout << "Total paths to fft: " << pathsToFft << std::endl;


    network = new Network();
    network->setStartServer("fft");

    for (int step = 0; step < 30; step++) {
        network->step(servers, "dac");
    }

    network->mergeConnections();

    // // find paths from fft to dac
    boost::multiprecision::int128_t  pathsFromFftToDac = findAllConnectionsBetween("fft", "dac", network);
    std::cout << "Total paths from fft to dac: " << pathsFromFftToDac << std::endl;


    network = new Network();
    network->setStartServer("dac");

    for (int step = 0; step < 25; step++) {
        network->step(servers, "out");
    }

    network->mergeConnections();

    // // find paths from dac to out
    boost::multiprecision::int128_t  pathsFromDacToOut = findAllConnectionsBetween("dac", "out", network);
    std::cout << "Total paths from dac to out: " << pathsFromDacToOut << std::endl;

    boost::multiprecision::int128_t  firstHalf = pathsToFft * pathsFromFftToDac;
    std::cout << "First half: " << firstHalf << std::endl;
    boost::multiprecision::int128_t  totalPaths = firstHalf * pathsFromDacToOut;
    std::cout << "Total paths: " << totalPaths << std::endl;

}