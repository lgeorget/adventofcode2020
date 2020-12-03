#include <iostream>
#include <fstream>
#include <vector>

int main()
{
	std::vector<int> v;
	std::ifstream input{"input"};

	while (input) {
		int i;
		input >> i;
		v.push_back(i);
	}

	for (unsigned int i=0 ; i<v.size() ; i++) {
		for (unsigned int j=i ; j<v.size() ; j++) {
			if (v[i] + v[j] == 2020) {
				std::cout << (v[i] * v[j]) << std::endl;
				return 0;
			}
		}
	}
}
