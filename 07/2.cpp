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
#include <utility>
#include <functional>

using Color = std::string;
using Edge = std::pair<unsigned long, Color>;
using Graph = std::map<Color, std::vector<Edge>>;

int main()
{
	std::ifstream file{"input"};
	std::regex contain{"(\\d+) ([^,]+) bag"};

	Graph g;

	while (file) {
		std::string s;
		std::getline(file, s);

		if (!s.empty()) {
			Color container = s.substr(0, s.find(" bag"));
			std::vector<Edge> containees;
			auto b = std::sregex_iterator(s.begin(), s.end(), contain);
			std::transform(b, decltype(b)(), std::back_inserter(containees),
				[](auto&& match){ return Edge{std::stoul(match.str(1)), match.str(2)}; });
			g.emplace(std::move(container), std::move(containees));
		}
	}

	/*
	for (const auto& [color, contained] : g) {
		std::cout << "Color " << color << " contains directly: ";
		for (const auto& c : contained) {
			std::cout << c << ", ";
		}
		std::cout << std::endl;
	}
	*/

	std::forward_list<std::pair<unsigned long, Color>> nextToVisit{{1, "shiny gold"}};
	unsigned long total = 0ul;
	while (!nextToVisit.empty()) {
		unsigned long nb;
		Color c;
		std::tie(nb, c) = std::move(nextToVisit.front());
		nextToVisit.pop_front();

		auto it = g.find(c);
		std::cerr << "Visiting " << nb << " * " << c << std::endl;
		if (it != g.end()) {
			std::transform(
				it->second.begin(),
				it->second.end(),
				std::front_inserter(nextToVisit),
				[nb](auto&& elt){ return std::make_pair(nb * elt.first, elt.second); }
			);
			std::transform(
				nextToVisit.begin(),
				nextToVisit.end(),
				std::ostream_iterator<std::string>(std::cerr, ", "),
				[](auto&& elt){ return std::to_string(elt.first) + " * " + elt.second; }
			);
			std::cerr << std::endl;
		}
		total += nb;
	}

	// The total must not include the shiny gold outermost bag itself
	std::cout << (total - 1) << std::endl;
}

