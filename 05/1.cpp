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

		unsigned int id = ((s[0] == 'B') << 9) +
				  ((s[1] == 'B') << 8) +
				  ((s[2] == 'B') << 7) +
				  ((s[3] == 'B') << 6) +
				  ((s[4] == 'B') << 5) +
				  ((s[5] == 'B') << 4) +
				  ((s[6] == 'B') << 3) +
				  ((s[7] == 'R') << 2) +
				  ((s[8] == 'R') << 1) +
				  ((s[9] == 'R') << 0)

		if (id > max)
			max = id;
	}

	std::cout << max << std::endl;
}
