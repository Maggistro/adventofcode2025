#include <string>
#include <vector>
#include <set>

class Server
{
    private:
        std::vector<std::string> connections;
        std::string name;
        bool visited;
        bool fft;
        bool dac;

    public:
        Server(std::string name, std::vector<std::string> connections)
        {
            this->name = name;
            this->connections = connections;
            this->visited = false;
            this->fft = false;
            this->dac = false;
        }

        std::string getName()
        {
            return this->name;
        }

        std::vector<std::string> getConnections()
        {
            return this->connections;
        }

        bool isVisited()
        {
            return this->visited;
        }

        void setVisited(bool visited)
        {
            this->visited = visited;
        }

        bool isFft()
        {
            return this->fft;
        }
        
        void setFft(bool fft)
        {
            this->fft = fft;
        }
        
        bool isDac()
        {
            return this->dac;
        }

        void setDac(bool dac)
        {
            this->dac = dac;
        }
};