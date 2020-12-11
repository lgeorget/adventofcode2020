#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include <string_view>

constexpr unsigned int COLUMNS = 91;
constexpr unsigned int ROWS = 98;

using Area = char[ROWS][COLUMNS];


bool updateSeats(const Area& currentArea, Area& newArea)
{
	bool changed = false;
	for (int r=0 ; r<ROWS ; r++) {
		for (int c=0 ; c<COLUMNS ; c++) {
			unsigned int occupied = 0;
			if (currentArea[r][c] == '.')
				continue;
			if (r > 0 && c > 0 && currentArea[r-1][c-1] == '#')
				occupied++;
			if (r > 0 && currentArea[r-1][c] == '#')
				occupied++;
			if (r > 0 && c < COLUMNS-1 && currentArea[r-1][c+1] == '#')
				occupied++;
			if (c > 0 && currentArea[r][c-1] == '#')
				occupied++;
			if (c < COLUMNS-1 && currentArea[r][c+1] == '#')
				occupied++;
			if (r < ROWS-1 && c > 0 && currentArea[r+1][c-1] == '#')
				occupied++;
			if (r < ROWS-1 && currentArea[r+1][c] == '#')
				occupied++;
			if (r < ROWS-1 && c < COLUMNS-1 && currentArea[r+1][c+1] == '#')
				occupied++;
			if (occupied == 0)
				newArea[r][c] = '#';
			else if (occupied >= 4)
				newArea[r][c] = 'L';
			else
				newArea[r][c] = currentArea[r][c];
			if (newArea[r][c] != currentArea[r][c])
				changed = true;
		}
	}
	return changed;
}

void printArea(const Area& a)
{
	for (unsigned int r=0 ; r<ROWS ; r++) {
		std::cout << std::string_view{a[r], COLUMNS} << '\n';
	}
}



int main()
{
	Area area;
	{
		std::ifstream input{"input"};
		for (unsigned int r=0 ; r<ROWS ; r++) {
			input.read(area[r], COLUMNS);
			input.get(); // eat the line return
		}
	}

	Area buffer;

	char* areaIt = reinterpret_cast<char*>(area);
	char* bufferIt = reinterpret_cast<char*>(buffer);

	bool changed = false;
	std::copy(areaIt, areaIt + ROWS * COLUMNS, bufferIt);
	do {
		printArea(area);
		std::cout << std::endl;
		changed = updateSeats(area, buffer);
		std::swap(area, buffer);
	} while (changed);

	std::cout << std::count(areaIt, areaIt + ROWS * COLUMNS, '#') << std::endl;

}
