#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <forward_list>
#include <map>
#include <regex>
#include <set>
#include <algorithm>
#include <iterator>

using Color = std::string;
using Graph = std::map<Color, std::vector<Color>>;

int main()
{
	std::ifstream file{"input"};
	std::regex contain{"\\d+ ([^,]+) bag"};

	Graph g;

	while (file) {
		std::string s;
		std::getline(file, s);

		if (!s.empty()) {
			Color container = s.substr(0, s.find(" bag"));
			std::vector<Color> containees;
			auto b = std::sregex_iterator(s.begin(), s.end(), contain);
			for (std::sregex_iterator i = b ; i != std::sregex_iterator() ; ++i) {
				Color contained = i->str(1);
				auto it = g.find(contained);
				if (it == g.end())
					g.emplace(std::move(contained), std::vector<Color>{container});
				else
					it->second.push_back(container);
			}
		}
	}

	/*
	for (const auto& [color, contained] : g) {
		std::cout << "Color " << color << " is contained directly in : ";
		for (const auto& c : contained) {
			std::cout << c << ", ";
		}
		std::cout << std::endl;
	}
	*/

	std::set<Color> solution;
	std::forward_list<Color> nextToVisit{"shiny gold"};
	while (!nextToVisit.empty()) {
		Color next = std::move(nextToVisit.front());
		nextToVisit.pop_front();

		auto it = g.find(next);
		std::cerr << "Visiting " << next << std::endl;
		if (it != g.end()) {
			std::copy(it->second.begin(), it->second.end(), std::inserter(solution, solution.end()));
			std::copy(it->second.begin(), it->second.end(), std::front_inserter(nextToVisit));
			std::copy(nextToVisit.begin(), nextToVisit.end(), std::ostream_iterator<Color>(std::cerr, ", "));
		}
	}

	std::copy(solution.begin(), solution.end(), std::ostream_iterator<Color>(std::cerr, ", "));
	std::cerr << std::endl;
	std::cout << solution.size() << std::endl;
}

