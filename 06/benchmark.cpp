#include <vector>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

int main()
{
	std::vector<unsigned long> ctimes2(1000);
	std::vector<unsigned long> wtimes2(1000);
	std::vector<unsigned long> ctimes3(1000);
	std::vector<unsigned long> wtimes3(1000);

	for (unsigned int i=0 ; i<1000 ; i++) {
		std::clock_t c_start = std::clock();
		auto t_start = std::chrono::high_resolution_clock::now();
		std::system("./2");
		std::clock_t c_end = std::clock();
		auto t_end = std::chrono::high_resolution_clock::now();
		ctimes2[i] = c_end - c_start;
		wtimes2[i] = (t_end - t_start).count();

		c_start = std::clock();
		t_start = std::chrono::high_resolution_clock::now();
		std::system("./3");
		c_end = std::clock();
		t_end = std::chrono::high_resolution_clock::now();
		ctimes3[i] = c_end - c_start;
		wtimes3[i] = (t_end - t_start).count();
	}

	std::ofstream times_out{"times"};
	times_out << "ctimes2\tctimes3\twtimes2\twtimes3\n";
	for (unsigned int i=0 ; i<1000 ; i++) {
		times_out << ctimes2[i] << "\t"
			  << ctimes3[i] << "\t"
			  << wtimes2[i] << "\t"
			  << wtimes3[i] << "\n";
	}
}
