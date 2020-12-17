#include <iostream>
#include <fstream>
#include <algorithm>
#include <string_view>

constexpr int SIZE=8;
constexpr int TURNS=7;

char cubes[1+TURNS*2][SIZE+TURNS*2][SIZE+TURNS*2];
char buffer[1+TURNS*2][SIZE+TURNS*2][SIZE+TURNS*2];

void print()
{
	for (unsigned int z=1 ; z<1+TURNS*2-1 ; z++) {
		std::cerr << '+' << std::string(SIZE+TURNS*2-2, '-') << "+\n";
		for (unsigned int y=1 ; y<SIZE+(TURNS-1)*2-1 ; y++) {
			std::cerr << "|";
			for (unsigned int x=1 ; x<SIZE+TURNS*2-1 ; x++) {
				std::cerr << cubes[z][y][x];
			}
			std::cerr << "|\n";
		}
		std::cerr << '+' << std::string(SIZE+TURNS*2-2, '-') << "+\n\n";
	}
}

int countAroundOn(int tz, int ty, int tx)
{
	int total=0;
	for (unsigned int z=tz-1 ; z<=tz+1 ; z++) {
		for (unsigned int y=ty-1 ; y<=ty+1 ; y++) {
			for (unsigned int x=tx-1 ; x<=tx+1 ; x++) {
				if (cubes[z][y][x] == '#')
					total++;
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
			input.read(cubes[TURNS][TURNS+i-1] + TURNS, SIZE);
			input.get(); // eat the line return
		}
	}

	for (unsigned int turn=0 ; turn<TURNS-1 ; turn++) {
		print();
		for (unsigned int z=1 ; z<1+TURNS*2-1 ; z++) {
			for (unsigned int y=1 ; y<SIZE+TURNS*2-1 ; y++) {
				for (unsigned int x=1 ; x<SIZE+TURNS*2-1 ; x++) {
					int count = countAroundOn(z,y,x);
					if (cubes[z][y][x] == '#' && count == 4) { // on
						buffer[z][y][x] = '#';
					} else if (count == 3) {
						buffer[z][y][x] = '#';
					} else {
						buffer[z][y][x] = '.';
					}
				}
			}
		}
		std::swap(cubes, buffer);
	}

	std::cout << std::count(reinterpret_cast<char*>(cubes), reinterpret_cast<char*>(cubes) + sizeof(cubes), '#') << std::endl;
}
