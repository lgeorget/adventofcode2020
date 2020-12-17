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

long int deduce(Matching& possibilities, const Ticket& mine)
{
	long int total = 1;
	int found = 0;
	Matching::iterator it = possibilities.begin();
	do {
		bool changed = false;
		it = std::find_if(it, possibilities.end(), [](auto&& p){ return p.size() == 1; });
		if (it != possibilities.end()) {
			for (auto it2 = possibilities.begin() ; it2 != possibilities.end() ; ++it2) {
				if (it != it2) {
					auto it3 = std::remove(it2->begin(), it2->end(), it->back());
					if (it3 != it2->end()) {
						changed = true;
						it2->erase(it3, it2->end());
					}
				}
			}
			if (changed) {
				if (it->back()->name.substr(0, 9) == "departure") {
					total *= mine[std::distance(possibilities.begin(), it)];
					found++;
				}
				it = possibilities.begin();
			} else {
				++it;
			}
		}
	} while (it != possibilities.end() && found < 6);

	return total;
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

	while (std::getline(input, line)) {
		std::istringstream ticketStream{line};
		std::string entry;

		Ticket t;
		while (std::getline(ticketStream, entry, ',')) {
			unsigned int value = std::stoul(entry);
			if (std::none_of(fields.begin(), fields.end(), [value](const Validity& v){ return v(value); }))
				break;
			t.push_back(value);
		}
		if (t.size() != possibilities.size()) // invalid ticket
			continue;

		for (unsigned int index=0 ; index<t.size() ; index++)
		{
			Matching::value_type& possibleFields = possibilities[index];
			possibleFields.erase(
				std::remove_if(possibleFields.begin(), possibleFields.end(),
					[value{t[index]}](const Validity* v) {
						return !(*v)(value);
					}
				),
				possibleFields.end()
			);
		}
	}

	std::cout << deduce(possibilities, mine) << std::endl;
}
