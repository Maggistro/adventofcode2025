#include <iostream>
#include <fstream>

using std::string;

int main() {
    std::ifstream in("../data");

    string line;
    int position = 50;
    int direction = 0; // -1 for left, 1 for right
    int distance = 0;
    int password = 0;
    int rollover = 0;

    while(std::getline(in, line))
    {
        direction = line[0] == 'L' ? -1 : 1;
        distance = stoi(line.substr(1));

        rollover = distance % 100;
        password += distance / 100;  // full rotation


        position += direction * rollover;

        if (position >= 100)
        {
            password++;
            position %= 100;
        }
        else if (position < 0)
        {
            if (rollover + position != 0) // only update if not started from 0
            {
                password++;
            }
            position += 100;
        } 
        else if (position == 0)
        {
            password++;
        }
    }

    std::cout << "\r\nPassword: " << password << std::endl;
    return 0;
}
