#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    std::ifstream in("../data");

    std::string line;

    getline(in, line);
    const int lineLength = line.length();

    const std::string rim = std::string(lineLength + 2, '.');
    const int lengthWithRim = lineLength + 2;

    in.clear();
    in.seekg(0);

    std::vector<char> buffer(0);
    buffer.insert(buffer.end(), rim.begin(), rim.end());

    while (getline(in, line)) {
        buffer.push_back('.');
        // std::cout << "Line: " << line << std::endl;
        buffer.insert(buffer.end(), line.begin(), line.end());
        buffer.push_back('.');
    }

    buffer.insert(buffer.end(), rim.begin(), rim.end());

    std::cout << "Buffer size: " << buffer.size() << std::endl;

    // std::cout << "\nBuffer: " << std::string(buffer.begin(), buffer.end()) << std::endl;

    int surrounding = 0;
    int possiblePaperRolls = 0;
    bool changed = false;

    do
    {
        changed = false;
        for (int position = 1 + lengthWithRim; position < buffer.size() - lengthWithRim - 1; ++position) {
            if (buffer[position] != '@') {
                continue;
            }
            // ascii value for '@' is 64, ascii value for '.' is 46
            // => 64*3 + 46*5 = 422 is maximuma allowed value for surrounding 8 positions
            surrounding =
                buffer[position - lengthWithRim - 1] +
                buffer[position - lengthWithRim] +
                buffer[position - lengthWithRim + 1] +
                buffer[position - 1] +
                buffer[position + 1] +
                buffer[position + lengthWithRim - 1] +
                buffer[position + lengthWithRim] +
                buffer[position + lengthWithRim + 1];
            // std::cout << "Position: " << position << " souroundingValue: " << surrounding << std::endl;

            if (surrounding <= 422)
            {
                ++possiblePaperRolls;
                buffer[position] = '.';
                changed = true;
            }
        }
    } while (changed);


    std::cout << "\npossiblePaperRolls: " << possiblePaperRolls << std::endl;

    return 0;
}