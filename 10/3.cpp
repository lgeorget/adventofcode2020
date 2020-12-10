#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <forward_list>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>

// With help from Reddit

struct Adapter {
	unsigned int value;
	unsigned long long paths;
};

bool operator<(const Adapter& e1, const Adapter& e2) {
	return e1.value < e2.value;
}

int operator-(const Adapter& e1, const Adapter& e2) {
	return e1.value - e2.value;
}

int main()
{
	std::vector<Adapter> adapters{Adapter{0, 1}};
	{
		std::ifstream input{"input"};
		std::transform(std::istream_iterator<unsigned int>{input},
			  std::istream_iterator<unsigned int>{},
			  std::back_inserter(adapters),
			  [](unsigned int v){ return Adapter{v, 0}; }
			 );
	}
	std::sort(adapters.begin(), adapters.end());
	adapters.push_back(Adapter{adapters.back().value + 3, 0});

	// Propagate the count of paths from nodes to nodes. We know an adapter
	// cannot possibly be connected to another farther from 3 nodes away in
	// the chain so we actually have a quite simple O(n) solution by doing
	// a 3-lookahead iteration on the vector.
	for (unsigned int i=0 ; i<adapters.size() - 1 ; i++) {
		adapters[i+1].paths += adapters[i].paths;
		if (i+2 < adapters.size() && adapters[i+2] - adapters[i] <= 3)
			adapters[i+2].paths += adapters[i].paths;
		if (i+3 < adapters.size() && adapters[i+3] - adapters[i] <= 3)
			adapters[i+3].paths += adapters[i].paths;
	}

	std::cout << adapters.back().paths << std::endl;
}
