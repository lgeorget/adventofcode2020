#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <functional>

// Just a type alias, you can change it to map to also store the fields' value
// separately from the header (you'll have to change the regex of course).
using Passport = std::vector<std::string>;

int main()
{
	unsigned int valids = 0;
	std::ifstream file{"input"};

	// We accept exactly one of any of the seven fields below provided that
	// it's immediately followed by a value that satisfies some arbitrary
	// condition, and a space or an end-of-line.
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

	Passport passport;
	passport.reserve(7);

	while (file) {
		std::string s;
		// read one line at a time
		std::getline(file, s);

		// An empty line marks the end of one passport in the input
		// file.
		// Don't forget to also check for the end of the file;
		// otherwise, the last entry will always be forgotten since it's
		// not followed by an empty line.
		if (file && !s.empty()) {
			// Insert all the substrings that match a passport
			// entry in the input line.
			std::transform(
				std::sregex_iterator(s.begin(), s.end(), entry),
				std::sregex_iterator(),
				std::back_inserter(passport),
				[](auto&& m){ return m.str(0); }
			);
		} else {
			// This is the end condition. Since only valid fields
			// are inserted into the passport, the only thing to
			// check for is that all of the required fields are
			// present.
			// Note that this code assumes that no field is present
			// more than once in any given passport.
			valids += passport.size() == 7;
			// Reset the passport to read the rest of the file.
			passport.clear();
		}
	}

	std::cout << valids << std::endl;
}
