#include <iostream>
#include <fstream>
#include <cmath>
#include <vector> 
#include <algorithm>

using std::vector;
using std::string;

bool isPrime(int number)
{
    double maxDenominator = sqrt(number);
    int denominator = 2;
    do
    {
        if (number % denominator == 0)
        {
            return false;
        }

        denominator++;
        if (denominator % 2 == 0)
        {
            denominator++;
        }

    } while (denominator <= maxDenominator);

    return true;
}

/**
 * Numbers can now have multiple centers. We need to calculate all common denominators.
 * E.g. a number with length 12 would have 2x6, 3x4, 4x3 and 6x2 blocks available
 *
 * 32 34 15 25
 * 32 32 32 32 => ignore
 * 32 34 32 34
 * 
 * 32 35 15 25
 * 32 32 32 32 => ignore
 * 32 35 32 35
 * 
 * ...
 * 
 * 35 34 15 25  
 * 35 35 35 35
 * 35 34 35 34
 * 
 * 35 35 15 25
 * 35 35 35 35 => ignore from before
 * 35 35 35 35 => ignore from before x 2
 * 
 * */ 
vector<long long> buildID(string start)
{
    long long startID = std::stoll(start);
    vector<long long> tempIds; // max 32 ids per number
    int blockSize = 0;

    for (int currentDenominator = 2; currentDenominator <= start.length(); currentDenominator++)
    {
        int idStorageIndex = 0;
        if (start.length() % currentDenominator != 0)
        {
            continue;
        }

        blockSize = start.length() / currentDenominator;

        long long id = 0;
        long long block = 0;
        
        for (int i = 0; i < currentDenominator; i++)
        {
            if (i == 0)
            {
                block = startID / pow(10,  start.length() - blockSize);
            }

            id += block * pow(10, i * blockSize);
        }
        
        tempIds.push_back(id);
        idStorageIndex++;
    }

    sort( tempIds.begin(), tempIds.end() );
    tempIds.erase( unique( tempIds.begin(), tempIds.end() ), tempIds.end() );

    return tempIds;
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

    std::ifstream in("../data");
    getline(in, line);

    do
    {
        currentDelimiter = line.find(',', lastDelimiter);
        
        token = line.substr(lastDelimiter, currentDelimiter - lastDelimiter);
        start = token.substr(0, token.find('-', 0));
        end = token.substr(token.find('-', 0) + 1);
    
        while (std::stoll(start) <= std::stoll(end))
        {

            // if length is prime, only single digits are valid
            if (isPrime(start.length()))
            {
                char first = start[0];
                id = std::stoll(string(start.length(), first));
                
                if (id <= std::stoll(end) && id >= std::stoll(start))
                {
                    std::cout << "  ID: " << id << "; ";
                    idSum += id;
                }

                if (first == '9')
                {
                    start = std::to_string(static_cast<long long>(pow(10, start.length())));
                }
                else
                {
                    start = std::to_string(id + static_cast<long long>(pow(10, start.length() - 1)));
                }
            }
            else
            {
                vector<long long> ids = buildID(start);

                for (int i = 0; i < ids.size(); i++)
                {
                    id = ids[i];

                    if (id <= std::stoll(end) && id >= std::stoll(start))
                    {
                        std::cout << "  ID: " << id << "; ";
                        idSum += id;
                    }                    
                }

                start = std::to_string(std::stoll(start) + static_cast<long long>(pow(10, start.length() / 2)));
            }

        }

        lastDelimiter = currentDelimiter + 1;
        // std::cout << "Token: " << token << " Start: " << start << " End: " << end << std::endl;
    } while (currentDelimiter != string::npos);

    std::cout << "\r\nID Sum: " << idSum << std::endl;

    return 0;
}