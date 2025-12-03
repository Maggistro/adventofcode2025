#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using std::string;


int main() {
    string line;
    std::ifstream in("../data");

    long long totalJoltage = 0;
    int currentDigit = 0;
    int restDigits = 12;
    char * joltage = new char[12];

    
    while(getline(in, line))
    {
        currentDigit = 0;
        restDigits = 11;


        std::vector <char> digits (line.begin(), line.end());

        for (int joltageDigit = 0; joltageDigit < 12; joltageDigit++)
        {

            for (int i = currentDigit; i < digits.size() - restDigits; i++)
            {
                if (digits.at(i) > joltage[joltageDigit])
                {
                    joltage[joltageDigit] = digits.at(i);
                    currentDigit = i + 1;
                }
            }

            restDigits--;
        }

        totalJoltage += std::stoll(std::string(joltage));

        std::cout << "Joltage " << joltage << " from " << std::string (digits.begin(), digits.end()) << std::endl;

        std::fill (joltage, joltage+12, '0');
    }

    std::cout << "\r\nTotal joltage: " << totalJoltage << std::endl;

    return 0;
}