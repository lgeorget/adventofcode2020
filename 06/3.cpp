#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>

int main()
{
	unsigned int total = 0;
	std::ifstream input{"input"};

	const std::vector<char> full =  {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
		'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
	};
	std::vector<char> answers = full;;
	bool firstOfGroup = true;

	while (input) {
		std::string form;
		std::getline(input, form);

		if (!input || form.empty()) {
			// end of group
			total += answers.size();
			answers = full;
		} else {
			std::vector<char> s;
			std::copy(form.begin(), form.end(), std::back_inserter(s));
			std::sort(s.begin(), s.end());

			auto it = std::set_intersection(
					answers.begin(), answers.end(),
					s.begin(), s.end(),
					answers.begin()
				);
			answers.erase(it, answers.end());
		}
	}

	std::cout << total << std::endl;
}
