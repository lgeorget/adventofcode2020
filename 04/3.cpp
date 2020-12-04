#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <algorithm>
#include <iterator>

using Passport = std::map<std::string, std::string>;

int main()
{
	unsigned int valids = 0;
	std::ifstream file{"input"};

	const std::regex entry(
		"(?:"
			"(?:(byr):(19[2-9][0-9]|200[0-2]))|"
			"(?:(iyr):(201[0-9]|2020))|"
			"(?:(eyr):(202[0-9]|2030))|"
			"(?:(hgt):((?:59|6[0-9]|7[0-6])in|(?:1[5-8][0-9]|19[0-3])cm))|"
			"(?:(hcl):(#[0-9a-f]{6}))|"
			"(?:(ecl):(amb|blu|brn|gry|grn|hzl|oth))|"
			"(?:(pid):([0-9]{9}))"
		")"
		"(?: |$)"
	);

	Passport passport;
	while (file) {
		std::string s;
		std::getline(file, s);

		if (!file || s.empty()) {
			std::cout << "Passport contains (valid fields only): ";
			for (const auto& [field, value] : passport)
				std::cout << field << ":" << value << " ";
			std::cout << std::endl;
			valids += passport.size() == 7;
			passport.clear();
		} else {
			auto b = std::sregex_iterator(s.begin(), s.end(), entry);
			for (std::sregex_iterator i = b ; i != std::sregex_iterator() ; ++i) {
				const std::smatch& field = *i;
				for (int j=1 ; j<field.size() ; j+=2) { // sub-matches go by pair
					if (field[j].matched) {
						passport.emplace(field[j], field[j+1]);
						break;
					}
				}
			}
		}
	}
	std::cout << valids << std::endl;
}
