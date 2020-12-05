#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>

int main()
{
	std::ifstream input{"input"};

	std::set<unsigned int> seats;
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
		seats.insert(id);
	}

	// We take two iterators on the set (which has the very nice property
	// of being sorted). _it2_ is always one step ahead of _it_. We stop the
	// iteration when we detect a "hole" of exactly one seat between _it_
	// and _it2_ (i.e. *it2 == *it + 2).
	for (auto it = seats.begin(), it2 = std::next(it) ; it2 != seats.end() ; ++it, ++it2) {
		if (*it2 == *it + 2) {
			std::cout << (*it + 1) << std::endl;
			break;
		}
	}
}
