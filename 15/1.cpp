#include <iostream>
#include <map>

using Memory = std::map<int, int>;

int main(int argc, char** argv)
{
	Memory mem;
	long int turn;
	int lastSpoken;

	long int lastTurn = std::stoi(argv[1]);

	for (turn=1 ; turn<argc-2 ; turn++) {
		int n = std::stoi(argv[turn+1]);
		std::cerr << "starting number: " << n << std::endl;
		mem.emplace(n, turn);
	}

	// the last argument is not inserted in memory right away, it's
	// part of the regular turns
	lastSpoken = std::stoi(argv[turn+1]);
	turn++;
	std::cerr << "starting number: " << lastSpoken << std::endl;
	for ( ; turn<=lastTurn ; turn++) {
		auto it = mem.find(lastSpoken);
		// We have to check the value of _it_ right away because
		// inserting a new value for lastSpoken would invalidate it (it
		// would always return the emplacement just added into the map).
		int newlySpoken = it == mem.end() ? 0 : (turn - 1 - it->second);
		mem.insert_or_assign(lastSpoken, turn-1);
		lastSpoken = newlySpoken;
		//std::cerr << lastSpoken << std::endl;
		if (turn % 100000 == 0)
			std::cerr << "Turn " << turn << " and playing..." << std::endl;
	}

	std::cout << lastSpoken << std::endl;
}
