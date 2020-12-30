#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <algorithm>
#include <functional>

struct Coordinate {
	int x;
	int y;

	void operator+=(Coordinate c) {
		x += c.x;
		y += c.y;
	}
};

bool operator<(Coordinate c1, Coordinate c2) {
	return c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y);
}

bool operator==(Coordinate c1, Coordinate c2) {
	return c1.x == c2.x && c1.y == c2.y;
}

struct Tileset {
	std::map<Coordinate, bool> t;
	static constexpr std::array<Coordinate, 6> NEIGHBOURS{ {{-2,0}, {2,0}, {-1,1}, {1,1}, {-1,-1}, {1,-1}} };

	void flip(Coordinate c) {
		auto it = t.find(c);
		if (it == t.end()) {
			t.emplace_hint(it, c, true);
		} else {
			it->second = !it->second;
		}
	}

	bool mustFlip(Coordinate c) {
		int blacks = 0;
		for (Coordinate neighbour : NEIGHBOURS) {
			neighbour += c;
			auto it = t.find(neighbour);
			blacks += it != t.end() && it->second;
		}

		auto it = t.find(c);
		bool black = it != t.end() && it->second;
		return (black && (blacks == 0 || blacks > 2)) || (!black && blacks == 2);
	}
};

int main()
{
	Tileset tiles, buffer;
	std::ifstream input{"input"};

	std::string s;
	while (std::getline(input, s)) {
		Coordinate c = {0,0};
		auto it = s.begin();
		while (it != s.end()) {
			if (*it == 'e') {
				c += {2,0};
			} else if (*it == 'w') {
				c += {-2,0};
			} else if (*it == 's') {
				++it;
				if (*it == 'w') {
					c += {-1,-1};
				} else {
					c += {1,-1};
				}
			} else if (*it == 'n') {
				++it;
				if (*it == 'w') {
					c += {-1,1};
				} else {
					c += {1,1};
				}
			}
			++it;
		}
		tiles.flip(c);
	}
	// end of initialization


	for (int day=1 ; day<=100 ; ++day) {
		for (auto p : tiles.t) {
			buffer.t[p.first] = p.second ^ tiles.mustFlip(p.first);
			for (Coordinate n : Tileset::NEIGHBOURS) {
				n += p.first;
				if (tiles.t.count(n) == 0)
					buffer.t[n] = tiles.mustFlip(n);
			}
		}
		tiles = std::move(buffer);
		std::cout << "Day " << day << ": "
			  << std::count_if(tiles.t.begin(), tiles.t.end(), [](const auto& p){ return p.second; })
			  << std::endl;
	}
}
