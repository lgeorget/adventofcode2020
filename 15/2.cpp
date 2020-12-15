#include <iostream>
#include <vector>

using Memory = std::vector<int>;

int main(int argc, char** argv)
{
	long int turn;
	int lastSpoken;

	long int lastTurn = std::stoi(argv[1]);
	Memory mem(lastTurn, 0);

	for (turn=1 ; turn<argc-2 ; turn++) {
		int n = std::stoi(argv[turn+1]);
		//std::cerr << "starting number: " << n << std::endl;
		mem[n] = turn;
	}

	// the last argument is not inserted in memory right away, it's
	// part of the regular turns
	lastSpoken = std::stoi(argv[turn+1]);
	turn++;
	//std::cerr << "starting number: " << lastSpoken << std::endl;
	for ( ; turn<=lastTurn ; turn++) {
		int newlySpoken = mem[lastSpoken] == 0 ? 0 : (turn - 1 - mem[lastSpoken]);
		mem[lastSpoken] = turn-1;
		lastSpoken = newlySpoken;
		//std::cerr << lastSpoken << std::endl;
		/* if (turn % 100000 == 0)
			std::cerr << "Turn " << turn << " and playing..." << std::endl; */
	}

	std::cout << lastSpoken << std::endl;
}
