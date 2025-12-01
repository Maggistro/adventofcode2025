#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream in("data");

    std::string line;
    int position = 50;
    int direction = 0; // -1 for left, 1 for right
    int distance = 0;
    int password = 0;

    while(std::getline(in, line))
    {
        direction = line[0] == 'L' ? -1 : 1;
        distance = std::stoi(line.substr(1));
        position += direction * distance;

        std::cout << "Current position: " << position << "Current command: " << line << std::endl;

        if (position >= 100)
        {
            position %= 100;
        }
        else
        {
            while (position < 0)
            {
                position += 100;
            }
        }
        
        if (position == 0)
        {
            password++;
        }
    }

    std::cout << "\r\nPassword: " << password << std::endl;
    return 0;
}
