#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

constexpr unsigned int LINES = 323;
constexpr unsigned int COLUMNS = 32;

int main()
{
	int fd = open("input", O_RDONLY);
	char* terrain = reinterpret_cast<char*>(
		mmap(nullptr, LINES * COLUMNS, PROT_READ, MAP_PRIVATE, fd, 0)
	);

	constexpr unsigned int move_r[] = {1, 3, 5, 7, 1};
	constexpr unsigned int move_d[] = {1, 1, 1, 1, 2};
	unsigned int total = 1;

	for (unsigned int attempt=0 ; attempt<5 ; attempt++) {
		unsigned int pos_r = 0;
		unsigned int pos_d = 0;

		unsigned int trees = 0;
		while (pos_d < LINES) {
			// the last column is the line return, so not part of the
			// terrain
			pos_r = (pos_r + move_r[attempt]) % (COLUMNS - 1);
			pos_d = pos_d + move_d[attempt];

			if (pos_d < LINES)
				trees += terrain[pos_d * COLUMNS + pos_r] == '#';
		}

		std::cout << trees << std::endl;
		total *= trees;
	}

	std::cout << total << std::endl;

	munmap(terrain, LINES * COLUMNS);
	close(fd);
}
