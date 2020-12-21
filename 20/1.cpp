#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <numeric>
#include <tuple>
#include <map>

constexpr int TILESIZE = 10;

struct Tile {
	char content[TILESIZE][TILESIZE];
	int borderMasks[4];
	int reversedBorderMasks[4];
	int id;

	std::tuple<int,int> computeMask(char* ptr, int jump) {
		int mask = 0;
		int reversedMask = 0;
		std::cerr << "border: ";
		for (int i=0 ; i<TILESIZE ; i++, ptr+=jump) {
			std::cerr << (*ptr);
			if (*ptr == '#') {
				mask |= (1 << i);
				reversedMask |= (1 << (TILESIZE - i - 1));
			}
		}
		std::cerr << "\n";
		return {mask, reversedMask};
	}

	Tile(int id, std::istream& is) : id{id} {
		for (int l=0 ; l<TILESIZE ; l++) {
			is.read(content[l], TILESIZE);
			is.get(); // eat the new line
		}

		std::cerr << "Tile " << id << ":\n";
		for (int i=0 ; i<TILESIZE ; i++) {
			std::cerr << std::string(content[i], TILESIZE) << "\n";
		}

		std::tie(borderMasks[0], reversedBorderMasks[0]) = computeMask(content[0], 1); // top border mask
		std::tie(borderMasks[1], reversedBorderMasks[1]) = computeMask(reinterpret_cast<char*>(content), TILESIZE); // left border mask
		std::tie(borderMasks[2], reversedBorderMasks[2]) = computeMask(content[TILESIZE-1], 1); // bottom border mask
		std::tie(borderMasks[3], reversedBorderMasks[3]) = computeMask(reinterpret_cast<char*>(content) + TILESIZE - 1, TILESIZE); // right border mask

		for (int i=0 ; i<4 ; i++) {
			std::cerr << borderMasks[i] << " " << reversedBorderMasks[i] << "\n";
		}
		std::cerr << std::endl;
	}

	int match(const Tile& t) {
		for (int i=0 ; i<4 ; i++) {
			for (int j=0 ; j<4 ; j++) {
				if (borderMasks[i] == t.borderMasks[j])
				       return (i+1)*10 + (j+1);
				else if (borderMasks[i] == t.reversedBorderMasks[j]) {
					return 100 + (i+1)*10 * (j+1);
				}
				//std::cerr << "Matching " << borderMasks[i] << " against " << t.borderMasks[j] << " or " << t.reversedBorderMasks[j] << std::endl;
			}
		}
		return 0;
	}
};

int main()
{
	std::ifstream input{"input"};
	std::vector<Tile> tiles;

	std::string line;
	while (std::getline(input, line)) {
		if (line.empty())
			continue;
		if (line.find("Tile") == 0) {
			int id = std::atoi(line.data() + 5);
			tiles.emplace_back(id, input);
		}
	}

	unsigned long int total = 1lu;
	std::map<int, int> matchingBorders;
	for (const Tile& t : tiles)
		matchingBorders.emplace(t.id, 0);

	for (auto ti = tiles.begin() ; ti != tiles.end() ; ++ti) {
		for (auto tj = std::next(ti) ; tj != tiles.end() ; ++tj) {
			int m = ti->match(*tj);
			if (m) {
				std::cerr << "Matched " << ti->id << " with " << tj->id << " " << m << std::endl;
				auto it = matchingBorders.find(ti->id);
				matchingBorders.insert_or_assign(it, ti->id, it->second+1);
				it = matchingBorders.find(tj->id);
				matchingBorders.insert_or_assign(it, tj->id, it->second+1);
			}
		}
	}

	for (const auto& [tile, matched] : matchingBorders) {
		if (matched == 2) {
			std::cerr << "Tile " << tile << " has " << matched << " borders." << std::endl;
			total *= tile;
		}
	}

	std::cout << total << std::endl;
}
