#include <iostream>
#include <fstream>
#include <map>
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

	void toggle(Coordinate c) {
		auto it = t.find(c);
		if (it == t.end()) {
			t.emplace_hint(it, c, true);
		} else {
			it->second = !it->second;
		}
	}
};

int main()
{
	Tileset tiles;
	std::ifstream input{"input"};

	std::string s;
	while (std::getline(input, s)) {
		std::cerr << s << std::endl;
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
		std::cerr << "Coordinate: (" << c.x << "," << c.y << ")" << std::endl;
		tiles.toggle(c);

		for (auto p : tiles.t) {
			std::cerr << "(" << p.first.x << "," << p.first.y << "):" << p.second << std::endl;
		}
	}

	std::cout << std::count_if(tiles.t.begin(), tiles.t.end(), [](const auto& p){ return p.second; }) << std::endl;
}
