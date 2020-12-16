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
	std::string name;
	unsigned int b1;
	unsigned int e1;
	unsigned int b2;
	unsigned int e2;

	const static std::regex VALIDITY_MATCHER;

	Validity(const std::string& input)
	{
		std::smatch result;
		std::regex_match(input, result, VALIDITY_MATCHER);
		name = result.str(1);
		b1 = std::stoul(result.str(2));
		e1 = std::stoul(result.str(3));
		b2 = std::stoul(result.str(4));
		e2 = std::stoul(result.str(5));
	}

	bool operator()(int value) const
	{
		return (value >= b1 && value <= e1) ||
		       (value >= b2 && value <= e2);
	}
};

using Ticket = std::vector<unsigned int>;
using Matching = std::vector<std::vector<const Validity*>>;

const std::regex Validity::VALIDITY_MATCHER{"([^:]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)"};

void simplify(Matching& possibilities)
{
	Matching::iterator it = possibilities.begin();
	do {
		bool changed = false;
		it = std::find_if(it, possibilities.end(), [](auto&& p){ return p.size() == 1; });
		if (it != possibilities.end()) {
			for (auto it2 = possibilities.begin() ; it2 != possibilities.end() ; ++it2) {
				if (it != it2) {
					auto it3 = std::remove(it2->begin(), it2->end(), it->back());
					if (it3 != it2->end()) {
						std::cerr << "Removed an exemplary of field " << it->back()->name << std::endl;
						changed = true;
						it2->erase(it3, it2->end());
					}
				}
			}
			if (changed)
				it = possibilities.begin();
			else
				++it;
		}
	} while (it != possibilities.end());
}

long int checkForEndCondition(Matching& possibilities, const Ticket& mine)
{
	long int total = 1;
	int found = 0;
	for (unsigned int i=0 ; i<possibilities.size() ; i++) {
		if (possibilities[i].size() == 1 && possibilities[i].back()->name.substr(0, 9) == "departure") {
			std::cerr << "Field " << possibilities[i].back()->name << " is resolved" << std::endl;
			found++;
			total *= mine[i];
		}
	}
	if (found == 6)
		return total;
	else
		return -1;
}

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
	std::sort(fields.begin(), fields.end(), [](const Validity& f1, const Validity& f2){ return f1.name < f2.name; });

	std::getline(input, line); // "your ticket:"
	std::getline(input, line); // the ticket
	Ticket mine;
	Matching possibilities;
	{
		std::istringstream ticketStream{line};
		std::string entry;
		while (std::getline(ticketStream, entry, ',')) {
			unsigned int value = std::stoul(entry);
			mine.push_back(value);
			Matching::value_type possibleFields;
			for (const Validity& v : fields) {
				if (v(value))
					possibleFields.push_back(&v);
			}
			possibilities.push_back(std::move(possibleFields));
		}
	}

	std::getline(input, line); // empty line
	std::getline(input, line); // "nearby tickets:"

	long int result = -1;
	while (result == -1 && std::getline(input, line)) {
		std::cerr << "New ticket " << line << std::endl;
		std::istringstream ticketStream{line};
		std::string entry;

		Ticket t;
		while (std::getline(ticketStream, entry, ',')) {
			unsigned int value = std::stoul(entry);
			if (std::none_of(fields.begin(), fields.end(), std::bind(&Validity::operator(), std::placeholders::_1, value))) {
				std::cerr << "Invalid ticket" << std::endl;
				break;
			}
			t.push_back(value);
		}
		if (t.size() != mine.size()) // invalid ticket
			continue;

		for (unsigned int index=0 ; index<t.size() ; index++)
		{
			unsigned int value = t[index];
			Matching::value_type& possibleFields = possibilities[index];
			auto it = std::remove_if(possibleFields.begin(), possibleFields.end(),
				[value](const Validity* v) {
					if (!(*v)(value)) {
						std::cerr << "Value " << value << " is not valid for field " << v->name << std::endl;
						return true;
					} else {
						return false;
					}
				}
			);
			if (it != possibleFields.end()) {
				std::cerr << "Removing " << std::distance(it, possibleFields.end()) << " fields at index " << index << std::endl;
				possibleFields.erase(it, possibleFields.end());
			}
		}
		simplify(possibilities);
		for (auto&& p : possibilities) {
			std::cerr << p.size() << " | ";
		}
		std::cerr << std::endl;

		result = checkForEndCondition(possibilities, mine);
	}

	std::cout << result << std::endl;
}
