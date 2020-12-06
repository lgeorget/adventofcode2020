#include <iostream>
#include <fstream>
#include <set>
#include <iterator>
#include <algorithm>

int main()
{
	unsigned int total = 0;
	std::ifstream input{"input"};

	std::set<char> answers;

	while (input) {
		std::string form;
		std::getline(input, form);

		if (!input || form.empty()) {
			// end of group
			total += answers.size();
			answers.clear();
		} else {
			std::copy(form.begin(), form.end(), std::inserter(answers, answers.end()));
		}
	}

	std::cout << total << std::endl;
}
