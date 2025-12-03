#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using std::string;


int main() {
    string line;
    std::ifstream in("../data");

    long totalJoltage = 0;
    int firstDigit = 0;
    int secondDigit = 1;

    
    while(getline(in, line))
    {
        firstDigit = 0;
        secondDigit = 1;
        std::vector <char> digits (line.begin(), line.end());

        for (int i = 0; i < digits.size() - 1; i++)
        {
            if (digits.at(i) > digits.at(firstDigit))
            {
                firstDigit = i;
            }
        }

        secondDigit = firstDigit + 1;
        
        for (int i = firstDigit + 1; i < digits.size(); i++)
        {
            if (digits.at(i) > digits.at(secondDigit))
            {
                secondDigit = i;
            }
        }


        totalJoltage += ((digits.at(firstDigit) - '0') * 10) + (digits.at(secondDigit) - '0');

        std::cout << "Joltage " << ((digits.at(firstDigit) - '0') * 10) + (digits.at(secondDigit) - '0') << " from " << std::string (digits.begin(), digits.end()) << std::endl;
    }

    std::cout << "\r\nTotal joltage: " << totalJoltage << std::endl;

    return 0;
}