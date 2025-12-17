#include <string>
#include <vector>
#include <set>

#include "Server.h"

Server::Server(std::string name, std::vector<std::string> connections)
{
    this->name = name;
    this->connections = connections;
    this->visited = false;
    this->fft = false;
    this->dac = false;
    this->visitCount = 0;
}

std::string Server::getName()
{
    return this->name;
}

std::vector<std::string> Server::getConnections()
{
    return this->connections;
}

bool Server::isVisited()
{
    return this->visited;
}

void Server::setVisited(bool visited)
{
    this->visited = visited;
}

bool Server::isFft()
{
    return this->fft;
}

void Server::setFft(bool fft)
{
    this->fft = fft;
}

bool Server::isDac()
{
    return this->dac;
}

void Server::setDac(bool dac)
{
    this->dac = dac;
}

int Server::getVisitCount()
{
    return this->visitCount;
}

void Server::incrementVisitCount()
{
    this->visitCount++;
}