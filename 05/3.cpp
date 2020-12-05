#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main()
{
	std::ifstream input{"input"};

	std::vector<unsigned int> seats;
	unsigned int max = 0;

	while (input) {
		std::string s;
		std::getline(input, s);
		if (s.empty())
			continue;

		unsigned int id = s[0] == 'B';
		for (unsigned int y=1 ; y<7 ; y++) {
			id = (id << 1) + (s[y] == 'B');
		}
		for (unsigned int x=0 ; x<3 ; x++) {
			id = (id << 1) + (s[7+x] == 'R');
		}

		if (id > max)
			max = id;
		seats.push_back(id);
	}

	std::sort(seats.begin(), seats.end());
	for (auto it = seats.begin(), it2 = std::next(it) ; it2 != seats.end() ; ++it, ++it2) {
		if (*it2 == *it + 2) {
			std::cout << (*it + 1) << std::endl;
			break;
		}
	}
}
