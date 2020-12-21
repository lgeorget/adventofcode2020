#include <iostream>
#include <fstream>

#include <array>
#include <vector>
#include <string>
#include <numeric>
#include <tuple>
#include <map>
#include <list>
#include <regex>

constexpr int TILESIZE = 10;
constexpr int SEA_MONSTER_LENGTH = 20;
constexpr int SEA_MONSTER_HEIGHT = 3;
constexpr int HASHES_IN_SEA_MONSTER = 15;

struct Tile {
	char content[TILESIZE][TILESIZE];
	int borderMasks[4];
	int reversedBorderMasks[4];
	int id;
	int x;
	int y;
	bool placed = false;

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

		recomputeMasks();

		for (int i=0 ; i<4 ; i++) {
			std::cerr << borderMasks[i] << " " << reversedBorderMasks[i] << "\n";
		}
		std::cerr << std::endl;
	}

	void recomputeMasks() {
		std::tie(borderMasks[0], reversedBorderMasks[0]) = computeMask(content[0], 1); // top border mask
		std::tie(borderMasks[1], reversedBorderMasks[1]) = computeMask(reinterpret_cast<char*>(content), TILESIZE); // left border mask
		std::tie(borderMasks[2], reversedBorderMasks[2]) = computeMask(content[TILESIZE-1], 1); // bottom border mask
		std::tie(borderMasks[3], reversedBorderMasks[3]) = computeMask(reinterpret_cast<char*>(content) + TILESIZE - 1, TILESIZE); // right border mask
	}

	int match(const Tile& t) {
		for (int i=0 ; i<4 ; i++) {
			for (int j=0 ; j<4 ; j++) {
				if (borderMasks[i] == t.borderMasks[j])
				       return (i+1)*10 + (j+1);
				else if (borderMasks[i] == t.reversedBorderMasks[j]) {
					return 100 + (i+1)*10 + (j+1);
				}
				//std::cerr << "Matching " << borderMasks[i] << " against " << t.borderMasks[j] << " or " << t.reversedBorderMasks[j] << std::endl;
			}
		}
		return 0;
	}

	void print() {
		for (int i=0 ; i<TILESIZE ; i++) {
			for (int j=0 ; j<TILESIZE ; j++) {
				std::cerr << content[i][j];
			}
			std::cerr << "\n";
		}
	}

	void vflip() {
		for (int i=0 ; i<TILESIZE/2 ; i++)
			std::swap(content[i], content[TILESIZE-1-i]);

		recomputeMasks();
	}

	void minus90() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[j][TILESIZE-1-i];
		std::swap(content, buffer);

		recomputeMasks();
	}

	void plus90flip() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[j][i];
		std::swap(content, buffer);

		recomputeMasks();
	}

	void plus90() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[TILESIZE-1-j][i];
		std::swap(content, buffer);

		recomputeMasks();
	}

	void flip() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[i][TILESIZE-1-j];
		std::swap(content, buffer);

		recomputeMasks();
	}

	void flipvflip() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[TILESIZE-1-i][TILESIZE-1-j];
		std::swap(content, buffer);

		recomputeMasks();
	}

	void minus90flip() {
		char buffer[TILESIZE][TILESIZE];
		for (int i=0 ; i<TILESIZE ; i++)
			for (int j=0 ; j<TILESIZE ; j++)
				buffer[i][j] = content[TILESIZE-1-j][TILESIZE-1-i];
		std::swap(content, buffer);

		recomputeMasks();
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

	std::list<int> toVisit = {0};
	tiles[0].x = 0;
	tiles[0].y = 0;
	tiles[0].placed = true;
	while (!toVisit.empty()) {
		int placed = toVisit.front();
		Tile& placedTile = tiles[placed];
		toVisit.pop_front();
		std::cerr << "Visiting tile " << placedTile.id << " at coordinates (" << placedTile.x << "," << placedTile.y << ")" << std::endl;

		for (int i=0 ; i<tiles.size() ; i++) {
			if (i == placed)
				continue;

			Tile& possible = tiles[i];
			if (possible.placed)
				continue;

			int m = placedTile.match(possible);
			if (!m)
				continue;

			// we have a winner
			std::cerr << "Matched " << placedTile.id << " with " << possible.id << " " << m << std::endl;
			switch (m) {
			case 11:
			case 33:
			case 124:
			case 142:
				possible.vflip();
				break;
			case 12:
			case 34:
			case 123:
			case 141:
				possible.minus90();
				break;
			case 14:
			case 23:
			case 32:
			case 41:
				possible.plus90flip();
				break;
			case 21:
			case 43:
			case 114:
			case 132:
				possible.plus90();
				break;
			case 22:
			case 44:
			case 113:
			case 131:
				possible.flip();
				break;
			case 111:
			case 122:
			case 133:
			case 144:
				possible.flipvflip();
				break;
			case 112:
			case 121:
			case 134:
			case 143:
				possible.minus90flip();
				break;
			case 13:
			case 24:
			case 31:
			case 42:
				/* nothing to do */
				break;
			}

			int dir = (m / 10) % 10;
			switch (dir) {
			case 1: // top border matched
				possible.x = placedTile.x;
				possible.y = placedTile.y - 1;
				break;
			case 2: // left border matched
				possible.x = placedTile.x - 1;
				possible.y = placedTile.y;
				break;
			case 3: // bottom border matched
				possible.x = placedTile.x;
				possible.y = placedTile.y + 1;
				break;
			case 4: // right border matched
				possible.x = placedTile.x + 1;
				possible.y = placedTile.y;
				break;
			}
			possible.placed = true;
			std::cerr << "	Tile " << possible.id << " is now placed at coordinates (" << possible.x << "," << possible.y << ")" << std::endl;

			toVisit.push_back(i);
		}
	}

	for (auto it = tiles.begin() ; it != tiles.end() ; it++) {
		std::cerr << "Tile " << it->id << (it->placed ? " placed" : "NOT PLACED") << ": (" << it->x << "," << it->y << ")" << std::endl;
	}
	std::cerr << "All pieces are in place" << std::endl;

	int minx = 0;
	int miny = 0;
	int maxx = 0;
	int maxy = 0;
	for (auto it = tiles.begin() ; it != tiles.end() ; it++) {
		if (it->x < minx)
			minx = it->x;
		if (it->y < miny)
			miny = it->y;
		if (it->x > maxx)
			maxx = it->x;
		if (it->y > maxy)
			maxy = it->y;
	}

	std::cerr << "Coordinates of image (" << tiles.size() << "): ( (" << minx << "," << miny << ") (" << maxx << "," << maxy << ") )" << std::endl;

	int sizex = maxx - minx + 1;
	int sizey = maxy - miny + 1;

	char img[sizey * (TILESIZE - 2)][sizex * (TILESIZE - 2)];
	std::fill(reinterpret_cast<char*>(img), reinterpret_cast<char*>(img) + (sizey * (TILESIZE - 2)) * (sizex * (TILESIZE - 2)), ' ');
	for (auto it = tiles.begin() ; it != tiles.end() ; it++) {
		for (int i=1 ; i<TILESIZE-1 ; i++) {
			for (int j=1 ; j<TILESIZE-1 ; j++) {
				img[ (it->y - miny) * (TILESIZE - 2) + i - 1 ]
				   [ (it->x - minx) * (TILESIZE - 2) + j - 1 ] =
				   it->content[i][j];
			}
		}
	}


	for (int i=0 ; i<sizey * (TILESIZE - 2) ; i++) {
		for (int j=0 ; j<sizex * (TILESIZE - 2) ; j++) {
			std::cerr << img[i][j];
		}
		std::cerr << "\n";
	}

	std::string offsetx = std::to_string(sizex * (TILESIZE - 2) - SEA_MONSTER_LENGTH);
	std::string offsety = std::to_string(sizex * (TILESIZE - 2) - SEA_MONSTER_HEIGHT);

	std::vector<std::regex> seaMonster{
		std::regex{
			"..................#." ".{"+offsetx+"}"
			"#....##....##....###" ".{"+offsetx+"}"
			".#..#..#..#..#..#..."
		},

		std::regex{
			".#.................." ".{"+offsetx+"}"
			"###....##....##....#" ".{"+offsetx+"}"
			"...#..#..#..#..#..#."
		},

		std::regex{
			".#..#..#..#..#..#..." ".{"+offsetx+"}"
			"#....##....##....###" ".{"+offsetx+"}"
			"..................#."
		},

		std::regex{
			"...#..#..#..#..#..#." ".{"+offsetx+"}"
			"###....##....##....#" ".{"+offsetx+"}"
			".#.................."
		},

		std::regex{
			".#." ".{"+offsety+"}"
			"##." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#."
		},

		std::regex{
			".#." ".{"+offsety+"}"
			".##" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#."
		},

		std::regex{
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..#" ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"##." ".{"+offsety+"}"
			".#."
		},

		std::regex{
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"..." ".{"+offsety+"}"
			"#.." ".{"+offsety+"}"
			".#." ".{"+offsety+"}"
			".##" ".{"+offsety+"}"
			".#."
		}
	};

	char* beginImg = reinterpret_cast<char*>(img);
	char* endImg = reinterpret_cast<char*>(img) + sizeof(img);

	int waterRoughness = std::count(beginImg, endImg, '#');
	std::cout << waterRoughness << std::endl;
	for (auto&& r : seaMonster) {
		std::cerr << "Next orientation: ";
		int seaMonstersFound = 0;
		char* start = beginImg;
		char* end = endImg;
		std::match_results<char*> monster;
		while (std::regex_search<char*>(start, end, monster, r)) {
			seaMonstersFound++;
			waterRoughness -= HASHES_IN_SEA_MONSTER;
			// look for the next monster on the same line as the one
			// just found, after min(SEA_MONSTER_HEIGHT, SEA_MONSTER_HEIGHT)
			// pixels (this assumes two horizontal monsters cannot be
			// matched one inside another)
			start = start + monster.position() + SEA_MONSTER_HEIGHT;
		}
		std::cerr << "found " << seaMonstersFound << std::endl;
	}
	std::cerr << std::endl;

	std::cout << waterRoughness << std::endl;
}
