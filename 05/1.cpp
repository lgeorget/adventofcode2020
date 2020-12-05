#include <iostream>
#include <fstream>

int main()
{
	std::ifstream input{"input"};

	unsigned int max = 0;

	while (input) {
		std::string s;
		std::getline(input, s);
		if (s.empty())
			continue;

		// Classical way of parsing a binary number (except that L and
		// F are 0, and B and R are 1)
		// Unifying the loops with (s[y] == 'B' || s[y] == 'R') might
		// be a false good idea because it means more useless
		// comparisons. (It probably would be a negligible cost with
		// respect to the reading of the file, though.)
		unsigned int id = s[0] == 'B';
		for (unsigned int y=1 ; y<7 ; y++) {
			id = (id << 1) + (s[y] == 'B');
		}
		for (unsigned int x=0 ; x<3 ; x++) {
			id = (id << 1) + (s[7+x] == 'R');
		}

		if (id > max)
			max = id;
	}

	std::cout << max << std::endl;
}
