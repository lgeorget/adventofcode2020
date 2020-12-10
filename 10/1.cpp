#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <numeric>

struct JoltageDiffs {
	unsigned int jolt1 = 0;
	unsigned int jolt3 = 0;

	unsigned int operator()() {
		return jolt1 * jolt3;
	}
};

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

	JoltageDiffs j{0, 0};
	std::adjacent_difference(adapters.cbegin(), adapters.cend(),
		JoltageDiffsIterator{j});
	std::cout << j() << std::endl;
}
