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

			int d = 1;
			while (r-d >= 0 && c-d >= 0 && currentArea[r-d][c-d] == '.')
				d++;
			if (r-d >= 0 && c-d >= 0 && currentArea[r-d][c-d] == '#')
				occupied++;

			d = 1;
			while (r-d >= 0 && currentArea[r-d][c] == '.')
				d++;
			if (r-d >= 0 && currentArea[r-d][c] == '#')
				occupied++;

			d = 1;
			while (r-d >= 0 && c+d < COLUMNS && currentArea[r-d][c+d] == '.')
				d++;
			if (r-d >= 0 && c+d < COLUMNS && currentArea[r-d][c+d] == '#')
				occupied++;

			d = 1;
			while (c-d >= 0 && currentArea[r][c-d] == '.')
				d++;
			if (c-d >= 0 && currentArea[r][c-d] == '#')
				occupied++;

			d = 1;
			while (c+d < COLUMNS && currentArea[r][c+d] == '.')
				d++;
			if (c+d < COLUMNS && currentArea[r][c+d] == '#')
				occupied++;

			d = 1;
			while (r+d < ROWS && c-d >= 0 && currentArea[r+d][c-d] == '.')
				d++;
			if (r+d < ROWS && c-d >= 0 && currentArea[r+d][c-d] == '#')
				occupied++;

			d = 1;
			while (r+d < ROWS && currentArea[r+d][c] == '.')
				d++;
			if (r+d < ROWS && currentArea[r+d][c] == '#')
				occupied++;

			d = 1;
			while (r+d < ROWS && c+d < COLUMNS && currentArea[r+d][c+d] == '.')
				d++;
			if (r+d < ROWS && c+d < COLUMNS && currentArea[r+d][c+d] == '#')
				occupied++;


			if (occupied == 0)
				newArea[r][c] = '#';
			else if (occupied >= 5)
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

	// that's probably fine...
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
