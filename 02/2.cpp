#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

int main()
{
	std::ifstream input{"input"};

	unsigned int corrects = 0;
	const std::regex entry("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
	while (input) {
		std::string s;
		std::getline(input, s);

		std::smatch match;
		if (std::regex_match(s, match, entry)) {
			if (match.size() == 5) {
				unsigned int pos1 = std::stoi(match[1].str());
				unsigned int pos2 = std::stoi(match[2].str());
				char letter = match[3].str()[0];
				std::string password = match[4].str();

				corrects += (password[pos1-1] == letter) ^ (password[pos2-1] == letter);
			}
		}
	}
	std::cout << corrects << std::endl;
}
