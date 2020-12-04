#include <iostream>
#include <fstream>
#include <set>
#include <regex>
#include <string>
#include <algorithm>

int main()
{
	unsigned int valids = 0;
	std::ifstream file{"input"};

	const std::regex entry("([a-z]+):[^ ]+");

	std::set<std::string> passport;
	while (file) {
		std::string s;
		std::getline(file, s);

		if (!file || s.empty()) {
			std::cout << "Passport contains: ";
			std::copy(passport.begin(), passport.end(),
					std::ostream_iterator<std::string>(std::cout, " "));
			std::cout << std::endl;
			// check the passport and reset the machine for the next
			if (passport.size() >= 7)
				valids += passport.count("byr") &&
					  passport.count("iyr") &&
					  passport.count("eyr") &&
					  passport.count("hgt") &&
					  passport.count("hcl") &&
					  passport.count("ecl") &&
					  passport.count("pid");
			passport.clear();
		} else {
			auto b = std::sregex_iterator(s.begin(), s.end(), entry);
			for (std::sregex_iterator i = b ; i != std::sregex_iterator() ; ++i) {
				const std::smatch& field = *i;
				if (field.size() == 2) {
					passport.insert(field[1]);
				}
			}
		}
	}
	std::cout << valids << std::endl;
}
