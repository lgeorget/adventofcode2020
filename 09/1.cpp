#include <iostream>
#include <fstream>
#include <array>

constexpr int BUFFER_SIZE = 25;

bool findPair(const std::array<int, BUFFER_SIZE>& buffer, int sum)
{
	for (int i=0 ; i<BUFFER_SIZE ; i++)
		for (int j=0 ; j<BUFFER_SIZE ; j++)
			if (i != j && buffer[i] + buffer[j] == sum)
				return true;
	return false;
}

int main()
{
	std::ifstream input{"input"};
	std::array<int, BUFFER_SIZE> buffer;
	for (int i=0 ; i<BUFFER_SIZE ; i++)
		input >> buffer[i];

	int bufferPos = 0;

	for (;;) {
		int entry;
		input >> entry;
		if (!input)
			break;

		if (!findPair(buffer, entry)) {
			std::cout << entry << std::endl;
			return 0;
		} else {
			buffer[bufferPos] = entry;
			bufferPos = (bufferPos + 1) % BUFFER_SIZE;
		}
	}
}
