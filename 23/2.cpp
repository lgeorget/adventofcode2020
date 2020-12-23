#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <iterator>

int main()
{
	// Our object is a list from MIN to MAX, which we won't manipulate as a
	// list at all...
	constexpr int MIN = 1;
	constexpr int MAX = 1'000'000;

	// The indexes in the array are the labels of the list elements (which
	// conveniently enough are numerical and contiguous), the values are
	// both the index and the label of the following element in the list.
	// We insert a dummy 0 element at the beginning because the elements are
	// labelled starting from MIN=1 to MAX, so I need them 1-indexed.
	// The sequence of elements is 4 6 9 2 1 7 5 3 8 followed by
	// incrementing numbers from 10 to MAX, so the element following 1 is 7,
	// the element following 2 is 1, the element following 3 is 8, etc.
	// hence the initialization below.
	std::array<int, MAX+1> input = { 0, 7, 1, 8, 6, 3, 9, 5, 10, 2 };
	// initialize the rest of the array
	std::iota(std::next(input.begin(), 10), input.end(), 11);
	// and loop back from the MAX element to the first
	input.back() = 4;

	// This is the first element of the list
	int current = 4;
	for (int turn=0 ; turn<10'000'000 ; turn++) {
		// The next element is the current one - 1, wrapped around MIN
		// to MAX.
		int selected = current - 1 < MIN ? MAX : current - 1;

		// Get the next three elements in the list, these are the ones
		// that will get moved
		auto pickedUp1 = input[current];
		auto pickedUp2 = input[pickedUp1];
		auto pickedUp3 = input[pickedUp2];

		// Make sure the selected element is not among the elements
		// about to be moved
		while (selected == pickedUp1 || selected == pickedUp2 || selected == pickedUp3) {
			selected = selected == MIN ? MAX : selected - 1;
		}

		// Move the slice consisting of the three picked up elements.
		// To move a portion of a linked list around, we only have three
		// pointers to change:
		// 1) the pointer from the previous element to the beginning of
		// the slice
		// 2) the pointer from the end of the slice to its next element
		// 3) the pointer from the insertion place to its next element.
		input[current] = input[pickedUp3];
		input[pickedUp3] = input[selected];
		input[selected] = pickedUp1; //i.e. the previous input[current]

		// If you want to show the current game state.
		/* int next = current;
		 * do {
		 *    std::cerr << next << " ";
		 *    next = input[next];
		 * } while (next != current);
		 * std::cerr << std::endl;
		 *
		 * if (i % 100'000 == 0)
		 *     std::cerr << "Turn " << i << " and going" << std::endl;
		 */

		// The next turn starts with the next element in the list.
		current = input[current];
	}

	// Get the label of the next two cups after "1" and multiply them:
	std::cerr << input[1] << " " << input[input[1]] << std::endl;
	unsigned long long int total = static_cast<unsigned long long int>(input[1]) * static_cast<unsigned long long int>(input[input[1]]);
	std::cout << total << std::endl;
}
