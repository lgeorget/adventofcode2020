#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>

int main()
{
	std::list<int> input = { 4,6,9,2,1,7,5,3,8 };
	int min = 1;
	int max = 9;

	for (int i=0 ; i<100 ; i++) {
		int current = input.front();
		int selected = current - 1 < min ? max : current - 1;

		auto pickedUpBegin = std::next(input.begin(), 1);
		auto pickedUpEnd   = std::next(input.begin(), 4);

		auto it = std::find(std::next(input.begin(), 4), input.end(), selected);
		while (it == input.end()) {
			selected = selected == min ? max : selected - 1;
			it = std::find(std::next(input.begin(), 4), input.end(), selected);
		}
		it++; // move the iterator after the element
		input.splice(it, input, pickedUpBegin, pickedUpEnd);

		// move the current element at the back
		input.pop_front();
		input.push_back(current);

		// show the current game state
		/*
		std::copy(input.begin(), input.end(), std::ostream_iterator<int>(std::cerr, " "));
		std::cerr << std::endl;
		*/
	}

	auto it = std::find(input.begin(), input.end(), 1);
	std::copy(std::next(it), input.end(), std::ostream_iterator<int>(std::cout, ""));
	std::copy(input.begin(), it, std::ostream_iterator<int>(std::cout, ""));
	std::cout << std::endl;
}
