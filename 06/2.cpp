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

	std::vector<char> answers;
	bool firstOfGroup = true;

	while (input) {
		std::string form;
		std::getline(input, form);

		if (!input || form.empty()) {
			// end of group
			total += answers.size();
			answers.clear();
			firstOfGroup = true;
		} else {
			std::vector<char> s;
			std::copy(form.begin(), form.end(), std::back_inserter(s));
			std::sort(s.begin(), s.end());

			if (firstOfGroup) {
				std::swap(answers, s);
				firstOfGroup = false;
			} else {
				auto it = std::set_intersection(
						answers.begin(), answers.end(),
						s.begin(), s.end(),
						answers.begin()
					);
				answers.erase(it, answers.end());
			}
		}
	}

	std::cout << total << std::endl;
}
