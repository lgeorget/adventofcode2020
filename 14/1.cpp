#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <string>
#include <string_view>
#include <numeric>

using Memory = std::unordered_map<unsigned int, uint64_t>;

int main()
{
	std::ifstream input{"input"};
	uint64_t mask_0{(uint64_t{1} >> 37) - 1};
	uint64_t mask_1{0u};
	Memory mem;

	std::string line;
	while (std::getline(input, line)) {
		if (line.find("mask = ") == 0) {
			// line looks like: mask = XX1X0X0XX1
			mask_0 = (uint64_t{1u} >> 37) - 1;
			mask_1 = 0u;
			for (int i=7 ; i<line.size() ; i++) {
				int bit = 35 - (i - 7);
				switch (line[i]) {
					case '0':
						mask_0 &= ~(uint64_t{1} << bit);
						break;
					case '1':
						mask_1 |= uint64_t{1} << bit;
						break;
				}
			}
			std::cerr << std::hex <<  "new mask_0 : " << mask_0 << " ; new mask_1 : " << mask_1 << std::dec << std::endl;
		} else {
			// line looks like: mem[1234] = 5678
			auto preMem = line.find('[');
			auto postMem = line.find(']');
			auto preValue = line.find(" = ");

			unsigned int addr = std::stoul(line.substr(preMem+1, postMem-preMem));
			uint64_t value = std::stoull(line.substr(preValue+3, line.size()));

			std::cerr << "addr : " << addr << " ; value = " << value << " ; masked value " << ((value & mask_0) | mask_1) << std::endl;
			mem.insert_or_assign(addr, (value & mask_0) | mask_1);
		}
	}

	uint64_t total{0};
	for (const auto& [addr, value] : mem) {
		std::cerr << "addr : " << addr << " ; value = " << value << std::endl;
		total += value;
	}
	std::cout << total << std::endl;
}
