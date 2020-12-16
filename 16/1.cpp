#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <vector>
#include <functional>
#include <algorithm>

struct Validity
{
	unsigned int b1;
	unsigned int e1;
	unsigned int b2;
	unsigned int e2;

	const static std::regex VALIDITY_MATCHER;

	Validity(const std::string& input)
	{
		std::smatch result;
		std::regex_match(input, result, VALIDITY_MATCHER);
		b1 = std::stoul(result.str(1));
		e1 = std::stoul(result.str(2));
		b2 = std::stoul(result.str(3));
		e2 = std::stoul(result.str(4));
	}

	bool operator()(int value) const
	{
		return (value >= b1 && value <= e1) ||
		       (value >= b2 && value <= e2);
	}
};

const std::regex Validity::VALIDITY_MATCHER{"[^:]+: (\\d+)-(\\d+) or (\\d+)-(\\d+)"};

int main()
{
	std::ifstream input{"input"};

	std::vector<Validity> fields;
	std::string line;
	for (;;) {
		std::getline(input, line);
		if (line.empty())
			break;

		fields.emplace_back(line);
	}

	for (; line != "nearby tickets:" ; std::getline(input, line))
		;

	for (auto&& v : fields) {
		std::cerr << "Validity: " << v.b1 << "-" << v.e1 << " " << v.b2 << "-" << v.e2 << std::endl;
	}

	unsigned int total = 0;
	while (std::getline(input, line)) {
		std::istringstream lineStream{line};
		std::string entry;
		while (std::getline(lineStream, entry, ',')) {
			unsigned int value = std::stoul(entry);
			if (std::none_of(fields.begin(), fields.end(), std::bind(&Validity::operator(), std::placeholders::_1, value))) {
				std::cerr << "Invalid: " << value << std::endl;
				total += value;
			}
		}
	}

	std::cout << total << std::endl;
}
