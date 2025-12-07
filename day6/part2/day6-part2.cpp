#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

struct MathProblem
{
	std::vector<int> numbers;

	char operation;

	long long calculate()
	{
		long long result = 0;
		for (int number : numbers)
		{
			if (operation == '+')
			{
				result += number;
			}
			else if (operation == '*')
			{
				result = (result == 0 ? number : result * number);
			}
		}
		return result;
	}

	void addNumber(int number)
	{
		numbers.push_back(number);
	}

	void setOperation(char op)
	{
		operation = op;
	}
};

int main()
{
	std::fstream in("../data");

	std::vector<char> rawLines;
	std::vector<MathProblem> problems;
	int number;

	std::vector<long long> results;
	long long total = 0;
	int lines = 0;

	for (std::string line; std::getline(in, line);)
	{
		lines++;
		rawLines.insert(rawLines.end(), line.begin(), line.end());
	}
	int lineLength = rawLines.size() / lines;

	
	MathProblem problem = MathProblem();
	for (int column = 0; column < lineLength; ++column)
	{
		number = 0;

		for(int position = column; position < rawLines.size(); position += lineLength)
		{
			char c = rawLines[position];
			if (rawLines[position] == ' ')
			{
				continue;
			}

			if (rawLines[position] == '*' || rawLines[position] == '+')
			{
				problem.setOperation(rawLines[position]);
				continue;
			}

			if (number != 0)
			{
				number *= 10;
			}

			number += atoi(&c);
		}

		if (number == 0)
		{
			problems.push_back(problem);
			problem = MathProblem();
			continue;
		}
		else
		{
			problem.addNumber(number);
		}
	}
	problems.push_back(problem);

	long long result = 0;
	for (MathProblem& prob : problems)
	{
		result = prob.calculate();
		results.push_back(result);
		total += result;
	}

	std::cout << "Total: " << total << std::endl;
}
