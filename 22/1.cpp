#include <iostream>
#include <fstream>

#include <list>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

int score(const std::list<int>& deck)
{
	return std::accumulate(deck.crbegin(), deck.crend(),
			std::make_pair(1, 0),
			[](std::pair<int, int> acc, int card) {
				acc.second += acc.first * card;
				acc.first++;
				return acc;
			}
		).second;
}


int main()
{
	std::ifstream input{"input"};

	std::list<int> deckPlayer1;
	std::list<int> deckPlayer2;

	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // "Player 1:"
	std::copy(std::istream_iterator<int>(input), std::istream_iterator<int>(), std::back_inserter(deckPlayer1));
	input.clear();
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // "Player 2:"
	std::copy(std::istream_iterator<int>(input), std::istream_iterator<int>(), std::back_inserter(deckPlayer2));

	std::cerr << "Decks:\n";
	std::copy(deckPlayer1.begin(), deckPlayer1.end(), std::ostream_iterator<int>(std::cerr, " "));
	std::cerr << "\n";
	std::copy(deckPlayer2.begin(), deckPlayer2.end(), std::ostream_iterator<int>(std::cerr, " "));
	std::cerr << std::endl;

	while (!deckPlayer1.empty() && !deckPlayer2.empty()) {
		int card1 = deckPlayer1.front();
		deckPlayer1.pop_front();
		int card2 = deckPlayer2.front();
		deckPlayer2.pop_front();

		if (card1 > card2) {
			deckPlayer1.push_back(card1);
			deckPlayer1.push_back(card2);
		} else { // this assumes equal cards never show up
			deckPlayer2.push_back(card2);
			deckPlayer2.push_back(card1);
		}

		std::cerr << "Decks:\n";
		std::copy(deckPlayer1.begin(), deckPlayer1.end(), std::ostream_iterator<int>(std::cerr, " "));
		std::cerr << "\n";
		std::copy(deckPlayer2.begin(), deckPlayer2.end(), std::ostream_iterator<int>(std::cerr, " "));
		std::cerr << std::endl;
	}

	int total = deckPlayer2.empty() ? score(deckPlayer1) : score(deckPlayer2);
	std::cout << total << std::endl;
}
