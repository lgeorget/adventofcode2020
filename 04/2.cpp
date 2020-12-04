#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <string>
#include <algorithm>
#include <iterator>

using Passport = std::map<std::string, std::string>;

bool validate(const Passport& p)
{
	if (p.size() != 7) {
		return false;
	}

	auto it = p.find("byr");
	unsigned int byr = std::stoul(it->second);
	if (byr < 1920 || byr > 2002) {
		return false;
	}

	it = p.find("iyr");
	unsigned int iyr = std::stoul(it->second);
	if (iyr < 2010 || iyr > 2020) {
		return false;
	}

	it = p.find("eyr");
	unsigned int eyr = std::stoul(it->second);
	if (eyr < 2020 || eyr > 2030) {
		return false;
	}

	it = p.find("hgt");
	unsigned int hgt = std::stoul(it->second);
	if (!(it->second[it->second.size()-2] == 'c' && it->second[it->second.size()-1] == 'm'
		&& hgt >= 150 && hgt <= 193) &&
	    !(it->second[it->second.size()-2] == 'i' && it->second[it->second.size()-1] == 'n'
		&& hgt >= 59 && hgt <= 76)) {
		return false;
	}

	// all other fields are already fully validated by the regex
	return true;
}


int main()
{
	unsigned int valids = 0;
	std::ifstream file{"input"};

	const std::regex entry(
		"(?:"
			"(?:(byr):([0-9]{4}))|"
			"(?:(iyr):([0-9]{4}))|"
			"(?:(eyr):([0-9]{4}))|"
			"(?:(hgt):([0-9]+(?:in|cm)))|"
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
			valids += validate(passport);
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
