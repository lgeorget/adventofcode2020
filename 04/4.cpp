#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <functional>

// This solution is very close to 3.cpp but stores only the count of valid
// fields in each passport, so there's no possibility to display them.
using Passport = unsigned int;

int main()
{
	unsigned int valids = 0;
	std::ifstream file{"input"};

	const std::regex entry(
		"(?:"
			"(?:byr:(?:19[2-9][0-9]|200[0-2]))|"
			"(?:iyr:(?:201[0-9]|2020))|"
			"(?:eyr:(?:202[0-9]|2030))|"
			"(?:hgt:(?:(?:59|6[0-9]|7[0-6])in|(?:1[5-8][0-9]|19[0-3])cm))|"
			"(?:hcl:#[0-9a-f]{6})|"
			"(?:ecl:(?:amb|blu|brn|gry|grn|hzl|oth))|"
			"(?:pid:[0-9]{9})"
		")"
		"(?: |$)"
	);

	Passport passport = 0;

	while (file) {
		std::string s;
		std::getline(file, s);

		if (file && !s.empty()) {
			// This function returns the number of times the regex
			// matched in the string
			passport += std::distance(
				std::sregex_iterator(s.begin(), s.end(), entry),
				std::sregex_iterator()
			);
		} else {
			valids += passport == 7;
			passport = 0;
		}
	}

	std::cout << valids << std::endl;
}
