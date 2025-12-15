#include <string>
#include <vector>

class Server
{
    private:
        std::vector<std::string> connections;
        std::string name;

    public:
        Server(std::string name, std::vector<std::string> connections)
        {
            this->name = name;
            this->connections = connections;
        }

        std::string getName()
        {
            return this->name;
        }

        std::vector<std::string> getConnections()
        {
            return this->connections;
        }
};