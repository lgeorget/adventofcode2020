#include <iostream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <limits>

int main()
{
	std::ifstream input{"input"};
	int earliest;
	input >> earliest;

	int min = std::numeric_limits<int>::max();
	int solution;

	std::string entry;
	while (std::getline(input, entry, ',')) {
		if (entry != "x") {
			int id = std::stoi(entry);
			int waitingTime = id - (earliest % id);
			if (waitingTime < min) {
				min = waitingTime;
				solution = waitingTime * id;
			}
		}
	}

	std::cout << solution << std::endl;
}

