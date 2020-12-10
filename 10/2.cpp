#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <forward_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>

using Graph = std::map<unsigned int, std::vector<unsigned int>>;

struct DFSVisitor {
	bool fresh;
	unsigned int name;
	unsigned int value;
	DFSVisitor* parent;
};

int main()
{
	std::vector<unsigned int> adapters{0};
	{
		std::ifstream input{"input"};
		std::copy(std::istream_iterator<unsigned int>{input},
			  std::istream_iterator<unsigned int>{},
			  std::back_inserter(adapters)
			 );
	}

	std::sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters.back() + 3);

	std::vector<unsigned int> cutpoints{0};

	Graph g;
	for (unsigned int i=0 ; i<adapters.size() - 1 ; i++) {
		std::vector<unsigned int> next{adapters[i+1]};
		if (i < adapters.size() - 2 && adapters[i+2] - adapters[i] <= 3)
			next.push_back(adapters[i+2]);
		if (i < adapters.size() - 3 && adapters[i+3] - adapters[i] <= 3)
			next.push_back(adapters[i+3]);

		if (adapters[i+1] - adapters[i] == 3)
			cutpoints.push_back(adapters[i+1]);

		g.emplace(adapters[i], std::move(next));
	}

	for (unsigned int i : adapters) {
		for (unsigned int k : g[i])
			std::cerr << i << " -> " << k << std::endl;
	}

	unsigned long long int total = 1u;

	for (unsigned int j=0 ; j<cutpoints.size() - 1 ; j++) {
		unsigned start = cutpoints[j];
		unsigned end = cutpoints[j+1];

		std::cerr << "Visiting subgraph from " << start << " to " << end << std::endl;

		std::forward_list<DFSVisitor> nexts{{true, start, 0, nullptr}};
		std::map<unsigned int, unsigned int> nbPaths;

		do {
			DFSVisitor& elt = nexts.front();
			std::cerr << "\tVisiting node " << elt.name << std::endl;

			if (!elt.fresh) {
				std::cerr << "\tThis is a node we've finished visiting" << std::endl;
				nbPaths[elt.name] = elt.value;
				elt.parent->value += elt.value;
				nexts.pop_front();
			} else if (elt.name == end) {
				std::cerr << "\tThis is the end node" << std::endl;
				elt.parent->value++;
				nexts.pop_front();
			} else {
				auto it = nbPaths.find(elt.name);
				if (it == nbPaths.end()) {
					std::cerr << "\tThis is a fresh node with descendants we've got to visit" << std::endl;
					for (unsigned int n : g[elt.name]) {
						nexts.push_front({true, n, 0, &elt});
					}
					elt.fresh = false;
				} else {
					std::cerr << "\tThis is a node for which we already know the number of paths" << std::endl;
					elt.parent->value += it->second;
					nexts.pop_front();
				}
			}
		} while (nexts.front().parent != nullptr);

		unsigned int partial = nexts.front().value;

		std::cerr << "Subgraph has " << partial << " paths" << std::endl;

		total *= partial;
	}

	std::cout << total << std::endl;
}
