#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <limits>

bool playGame(std::list<int>& deckPlayer1, std::list<int>& deckPlayer2);

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

bool playGame(std::list<int>& deckPlayer1, std::list<int>& deckPlayer2)
{
	std::vector<std::pair<std::list<int>, std::list<int>>> alreadyPlayed;

	while (!deckPlayer1.empty() && !deckPlayer2.empty()) {
/*		std::cerr << "Decks:\n";
		std::copy(deckPlayer1.begin(), deckPlayer1.end(), std::ostream_iterator<int>(std::cerr, " "));
		std::cerr << "\n";
		std::copy(deckPlayer2.begin(), deckPlayer2.end(), std::ostream_iterator<int>(std::cerr, " "));
		std::cerr << std::endl;
*/

		auto reminder = std::make_pair(deckPlayer1, deckPlayer2);
		if (std::find(alreadyPlayed.begin(), alreadyPlayed.end(), reminder) != alreadyPlayed.end())
			return true; // Player 1 wins automatically
		alreadyPlayed.emplace_back(reminder);

		int card1 = deckPlayer1.front();
		deckPlayer1.pop_front();
		int card2 = deckPlayer2.front();
		deckPlayer2.pop_front();

		if (deckPlayer1.size() >= card1 && deckPlayer2.size() >= card2) {
			std::list<int> d1;
			std::list<int> d2;
			std::copy(deckPlayer1.begin(), std::next(deckPlayer1.begin(), card1), std::back_inserter(d1));
			std::copy(deckPlayer2.begin(), std::next(deckPlayer2.begin(), card2), std::back_inserter(d2));
			if (playGame(d1, d2)) {
				deckPlayer1.push_back(card1);
				deckPlayer1.push_back(card2);
			} else {
				deckPlayer2.push_back(card2);
				deckPlayer2.push_back(card1);
			}
		} else if (card1 > card2) {
			deckPlayer1.push_back(card1);
			deckPlayer1.push_back(card2);
		} else {
			deckPlayer2.push_back(card2);
			deckPlayer2.push_back(card1);
		}
	}
	return deckPlayer2.empty();
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

	int total = playGame(deckPlayer1, deckPlayer2) ? score(deckPlayer1) : score(deckPlayer2);
	std::cout << total << std::endl;
}
