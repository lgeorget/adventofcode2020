#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

constexpr unsigned int MAGIC_SUM = 10884537u;

int main()
{
	std::ifstream input{"input"};
	std::vector<unsigned int> list;
	std::copy(std::istream_iterator<unsigned int>(input), std::istream_iterator<unsigned int>(), std::back_inserter(list));

	for (auto it = list.begin() ; it != list.end() ; it++) {
		for (auto it2 = std::next(it, 2) ; it2 != list.end() ; it2++) {
			unsigned int partialSum = std::accumulate(it, it2, 0);
			if (partialSum == MAGIC_SUM) {
				auto p = std::minmax_element(it, it2);
				std::cout << (*p.first + *p.second)  << std::endl;
				return 0;
			} else if (partialSum > MAGIC_SUM) {
				break;
			}
		}
	}
}
