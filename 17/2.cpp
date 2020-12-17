#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>

constexpr int SIZE=8;
constexpr int TURNS=7;

char cubes[1+TURNS*2][1+TURNS*2][SIZE+TURNS*2][SIZE+TURNS*2];
char buffer[1+TURNS*2][1+TURNS*2][SIZE+TURNS*2][SIZE+TURNS*2];

int countAroundOn(int tw, int tz, int ty, int tx)
{
	int total=0;
	for (unsigned int w=tw-1 ; w<=tw+1 ; w++) {
		for (unsigned int z=tz-1 ; z<=tz+1 ; z++) {
			for (unsigned int y=ty-1 ; y<=ty+1 ; y++) {
				for (unsigned int x=tx-1 ; x<=tx+1 ; x++) {
					if (cubes[w][z][y][x] == '#')
						total++;
				}
			}
		}
	}
	return total;
}

int main()
{
	std::fill(reinterpret_cast<char*>(cubes), reinterpret_cast<char*>(cubes) + sizeof(cubes), '.');
	std::fill(reinterpret_cast<char*>(buffer), reinterpret_cast<char*>(buffer) + sizeof(buffer), '.');
	{
		std::ifstream input{"input"};
		for (unsigned int i=0 ; i<SIZE ; i++) {
			input.read(cubes[TURNS][TURNS][TURNS+i-1] + TURNS, SIZE);
			input.get(); // eat the line return
		}
	}

	for (unsigned int turn=0 ; turn<TURNS-1 ; turn++) {
		for (unsigned int w=TURNS-turn-2 ; w<TURNS+turn+2 ; w++) {
			for (unsigned int z=TURNS-turn-2 ; z<TURNS+turn+2 ; z++) {
				for (unsigned int y=TURNS-turn-1 ; y<SIZE+TURNS+turn+1 ; y++) {
					for (unsigned int x=TURNS-turn-1 ; x<SIZE+TURNS+turn+1 ; x++) {
						int count = countAroundOn(w,z,y,x);
						if (cubes[w][z][y][x] == '#' && count == 4) { // on
							buffer[w][z][y][x] = '#';
						} else if (count == 3) {
							buffer[w][z][y][x] = '#';
						} else {
							buffer[w][z][y][x] = '.';
						}
					}
				}
			}
		}
		std::swap(cubes, buffer);
	}

	std::cout << std::count(reinterpret_cast<char*>(cubes), reinterpret_cast<char*>(cubes) + sizeof(cubes), '#') << std::endl;
}
