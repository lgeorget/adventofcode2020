#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>

struct Bus {
	int id;
	int waitingTime;
};

int main()
{
	std::ifstream input{"input"};
	int earliest;
	std::vector<Bus> buses;
	input >> earliest;
	while (input) {
		std::string entry;
		std::getline(input, entry, ',');
		if (input && entry != "x") {
			int id = std::stoi(entry);
			int waitingTime = id - (earliest % id);
			buses.push_back(Bus{id, waitingTime});
		}
	}

	auto it = std::min_element(buses.begin(), buses.end(),
			[](const Bus& b1, const Bus& b2){ return b1.waitingTime < b2.waitingTime; });

	std::cout << (it->id * it->waitingTime) << std::endl;
}

