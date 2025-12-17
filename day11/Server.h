#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <set>

class Server
{
    private:
        std::vector<std::string> connections;
        std::string name;
        int visitCount;
        bool visited;
        bool fft;
        bool dac;

    public:
        Server(std::string name, std::vector<std::string> connections);

        std::string getName();
        std::vector<std::string> getConnections();
        bool isVisited();
        void setVisited(bool visited);
        bool isFft();
        void setFft(bool fft);
        bool isDac();
        void setDac(bool dac);
        int getVisitCount();
        void incrementVisitCount();
};

#endif // SERVER_H
