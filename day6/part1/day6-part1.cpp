#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main()
{
	std::fstream in("../data");

	std::vector<std::string> lineNumbers;
	std::string number;

	std::vector<long long> results;
	long long total = 0;
	int lines = 0;

	for (std::string line; std::getline(in, line);)
	{
		lines++;
		number.clear();
		for(char c : line)
		{
			if (c == ' ')
			{
				if (number.empty())
				{
					continue;
				}

				lineNumbers.push_back(number);
				number.clear();
				continue;
			}
			else
			{
				number.push_back(c);
			}
		}

		if (!number.empty())
		{
			lineNumbers.push_back(number);
		}

	}
	int lineLength = lineNumbers.size() / lines;


	std::vector<char> operators;

	for (int current = lineLength; current > 0; --current)
	{
		operators.push_back(lineNumbers[lineNumbers.size() - current][0]);
	}

	std::cout << "\nOperators: " << operators.data() << std::endl;

	long long result = 0;
	for (int number = 0; number < lineLength; ++number)
	{
		result = 0;
		for (int line = 0 + number; line < lineNumbers.size() - lineLength; line += lineLength)
		{
			if ('+' == operators[number])
			{
				result += std::stoll(lineNumbers[line]);
			}
			else if ('*' == operators[number])
			{
				result = (result == 0 ? std::stoll(lineNumbers[line]) : result * std::stoll(lineNumbers[line]));
			}
		}
		results.push_back(result);
		total += result;
	}

	std::cout << "Total: " << total << std::endl;
}
