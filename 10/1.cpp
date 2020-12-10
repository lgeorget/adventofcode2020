#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>

// We define a custom type to store the number of adapters with difference of
// 1 jolt and 3 jolts.
struct JoltageDiffs {
	unsigned int jolt1 = 0;
	unsigned int jolt3 = 0;

	// For extra style points, we make this type callable so that the result
	// of the challenge can be obtained by calling it.
	unsigned int operator()() {
		return jolt1 * jolt3;
	}
};

// We define an output iterator suitable for std::adjacent_difference below
struct JoltageDiffsIterator {
	using iterator_category = std::output_iterator_tag;
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;
	using container_type = JoltageDiffs;

	JoltageDiffs& j;

	JoltageDiffsIterator(JoltageDiffs& joltageDiffs) : j{joltageDiffs} {}

	JoltageDiffsIterator& operator++() {
		return *this;
	}

	JoltageDiffsIterator& operator*() {
		return *this;
	}

	// As with all output iterators, the only important method is the
	// affectation
	// Here we take into account only differences of 1 and 3 and we add
	// them to the proper fields in the JoltageDiffs instance we're
	// "iterating" upon.
	JoltageDiffsIterator& operator=(unsigned int diff) {
		if (diff == 1)
			j.jolt1++;
		else if (diff == 3)
			j.jolt3++;
		return *this;
	}
};



int main()
{
	// Read numbers inside a vector, it already has got one element, the
	// charging outlet
	std::vector<unsigned int> adapters{0};
	{ // we open a scope to keep the file open only for the time we need it
		std::ifstream input{"input"};
		std::copy(std::istream_iterator<unsigned int>{input},
			  std::istream_iterator<unsigned int>{},
			  std::back_inserter(adapters)
			 );
	}

	// Sort the vector and add the last element, the power sink
	// Although it's not explicitly said in the challenge, it's clear that
	// the adapters need to be ordered by < to use them all.
	std::sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters.back() + 3);

	// Create an instance of JoltageDiffs to compute the answer
	JoltageDiffs j;
	// Magic happens here, we use the std::adjacent_difference standard
	// algorithm (which output the differences between consecutive elements)
	// but our custom iterator populates the JoltageDiffs instance in a
	// funny way.
	std::adjacent_difference(adapters.cbegin(), adapters.cend(),
		JoltageDiffsIterator{j});

	// Finally, we ask j to output the answer
	std::cout << j() << std::endl;
}
