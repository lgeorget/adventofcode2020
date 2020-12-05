#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

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

		unsigned int id = ((s[0] == 'B') << 9) +
				  ((s[1] == 'B') << 8) +
				  ((s[2] == 'B') << 7) +
				  ((s[3] == 'B') << 6) +
				  ((s[4] == 'B') << 5) +
				  ((s[5] == 'B') << 4) +
				  ((s[6] == 'B') << 3) +
				  ((s[7] == 'R') << 2) +
				  ((s[8] == 'R') << 1) +
				  ((s[9] == 'R') << 0);

		if (id > max)
			max = id;
		seats.push_back(id);
	}

	std::sort(seats.begin(), seats.end());
	auto it = std::adjacent_find(seats.begin(), seats.end(), [](int a, int b){ return b - a == 2; });
	if (it != seats.end())
		std::cout << (*it + 1) << std::endl;
}
