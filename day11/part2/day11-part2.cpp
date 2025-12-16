#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <set>

#include "../Server.cpp"

int traverseConnection(Server* currentServer, std::map<std::string, Server*>* servers, bool fft, bool dac, std::string target) {

    if (currentServer->isVisited())
    {
        if (target == "fft" && !fft)
        {
            return 1;
        }

        if (target == "dac" && !dac)
        {
            return 1;
        }

        return 0;
        // std::cout << "skipped" << std::endl;
    }

    int pathCount = 0;
    currentServer->setVisited(true);
    std::vector<std::string> connections = currentServer->getConnections();
    for (std::string connection : connections) 
    {
        if (connection == target) 
        {
            // std::cout << target << (fft && dac ? " true" : " false") << std::endl;
            // if (fft && dac) {
            //     pathCount++;
            // }
            pathCount++;
            continue;
        }

        if (connection == "out") {
            continue;
        }

        bool nextFft = fft || (connection == "fft");
        bool nextDac = dac || (connection == "dac");

        // Server* server = servers->at(connection);
        // if (server->isVisited()
        //     && !nextFft && !nextDac)
        // {
            // std::cout << "skipped" << std::endl;
        //     continue;
        // }

        // server->setFft(nextFft);
        // server->setDac(nextDac);

        // std::cout << connection << " ";
        pathCount += traverseConnection(servers->at(connection), servers, nextFft, nextDac, target);
    }

    return pathCount;
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

    // find paths from svr to fft
    std::string target = "fft";
    int pathsToFft = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths to fft: " << pathsToFft << std::endl;

    for (auto& [name, server] : servers) {
        server->setVisited(false);
    }

    // find paths from svr to dac
    target = "dac";
    int pathsToDac = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths to dac: " << pathsToDac << std::endl;

    for (auto& [name, server] : servers) {
        server->setVisited(false);
    }

    // find paths from fft to dac
    currentServer = servers.at("fft");
    target = "dac";
    int pathsFromFftToDac = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths from fft to dac: " << pathsFromFftToDac << std::endl;

    for (auto& [name, server] : servers) {
        server->setVisited(false);
    }

    // find paths from dac to fft
    currentServer = servers.at("dac");
    target = "fft";
    int pathsFromDacToFft = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths from dac to fft: " << pathsFromDacToFft << std::endl;

    for (auto& [name, server] : servers) {
        server->setVisited(false);
    }

    // find paths from fft to out
    currentServer = servers.at("fft");
    target = "out"; 
    int pathsFromFftToOut = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths from fft to out: " << pathsFromFftToOut << std::endl;

    for (auto& [name, server] : servers) {
        server->setVisited(false);
    }

    // find paths from dac to out
    currentServer = servers.at("dac");  
    target = "out";
    int pathsFromDacToOut = traverseConnection(currentServer, &servers, false, false, target);
    std::cout << "Total paths from dac to out: " << pathsFromDacToOut << std::endl;

    long long totalPaths = 
        (pathsToFft * pathsFromFftToDac * pathsFromDacToOut) +
        (pathsToDac * pathsFromDacToFft * pathsFromFftToOut) +
        (pathsToDac * pathsFromDacToFft * pathsFromFftToDac * pathsFromDacToOut) + 
        (pathsToDac * pathsFromFftToDac * pathsFromDacToFft * pathsFromDacToOut);

    std::cout << "Total paths: " << totalPaths << std::endl;

}