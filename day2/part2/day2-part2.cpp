#include <iostream>
#include <fstream>
#include <cmath>

using std::string;

long long buildID(string start)
{
    long long startID = std::stoll(start);
    long long secondHalf = startID / pow(10,  start.length() / 2);
    long long firstHalf = secondHalf * pow(10, start.length() / 2);

    return firstHalf + secondHalf;
}

int main() {
    string line;
    string token;
    int lastDelimiter = 0;
    int currentDelimiter = 0;
    int indexInID = 0;
    string start;
    string end;

    long long id = 0;
    long long idSum = 0;

    std::ifstream in("../test");
    getline(in, line);

    do
    {
        currentDelimiter = line.find(',', lastDelimiter);
        
        token = line.substr(lastDelimiter, currentDelimiter - lastDelimiter);
        start = token.substr(0, token.find('-', 0));
        end = token.substr(token.find('-', 0) + 1);
    
        while (std::stoll(start) <= std::stoll(end))
        {

            // if number has a odd length, only single digits are valid
            if (start.length() % 2 == 1)
            {
                char first = start[0];
                start = string(start.length(), first);
                id = std::stoll(start);
                

                if (id <= std::stoll(end) && id >= std::stoll(start))
                {
                    std::cout << "  ID: " << id << "; ";
                    idSum += id;
                }
                
                start = std::to_string(id + static_cast<long long>(pow(10, start.length() - 1)));
            }
            else
            {
                id = buildID(start);

                if (id <= std::stoll(end) && id >= std::stoll(start))
                {
                    std::cout << "  ID: " << id << "; ";
                    idSum += id;
                }
                
                start = std::to_string(id + static_cast<long long>(pow(10, start.length() / 2)));
            }

        }

        lastDelimiter = currentDelimiter + 1;
        // std::cout << "Token: " << token << " Start: " << start << " End: " << end << std::endl;
    } while (currentDelimiter != string::npos);

    std::cout << "\r\nID Sum: " << idSum << std::endl;

    return 0;
}