#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// I cheat a little, I should read the file to get those lengths...
constexpr unsigned int LINES = 323;
constexpr unsigned int COLUMNS = 32;

int main()
{
	int fd = open("input", O_RDONLY);
	char* terrain = reinterpret_cast<char*>(
		mmap(nullptr, LINES * COLUMNS, PROT_READ, MAP_PRIVATE, fd, 0)
	);
	// no need to check the return values of critical functions, who got
	// the time anyway
	
	unsigned int pos_r = 0;
	unsigned int pos_d = 0;

	unsigned int trees = 0;
	while (pos_d < LINES) {
		// the last column is the line return, so not part of the
		// terrain
		pos_r = (pos_r + 3) % (COLUMNS - 1);
		pos_d++;

		trees += terrain[pos_d * COLUMNS + pos_r] == '#';
	}

	std::cout << trees << std::endl;

	// we do, however, close everything, it's just the polite thing to do
	munmap(terrain, LINES * COLUMNS);
	close(fd);
}
