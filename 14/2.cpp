#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>

struct AltMask {
	uint64_t negative = (uint64_t{1} << 37) - 1;
	uint64_t positive = uint64_t{0};
};

using Memory = std::unordered_map<uint64_t, uint64_t>;
using FloatingBits = std::vector<AltMask>;

int main()
{
	std::ifstream input{"input"};
	uint64_t mask_1{0u};
	Memory mem;
	FloatingBits mask_f = {AltMask{}};

	std::string line;
	while (std::getline(input, line)) {
		if (line.find("mask = ") == 0) {
			// line looks like: mask = XX1X0X0XX1
			mask_1 = uint64_t{0};
			mask_f = {AltMask{}};
			for (int i=7 ; i<line.size() ; i++) {
				int bit = 35 - (i - 7);
				FloatingBits newMask;
				if (line[i] == 'X') {
					for (const AltMask& m : mask_f) {
						newMask.push_back(AltMask{m.negative & ~(uint64_t{1} << bit), m.positive});
						newMask.push_back(AltMask{m.negative, m.positive | (uint64_t{1} << bit)});
					}
					mask_f = std::move(newMask);
				} else if (line[i] == '1') {
					mask_1 |= uint64_t{1} << bit;
				}
			}
			std::cerr << std::hex <<  "new mask_1 : " << mask_1 << "\n"
				  << "In mask_f: \n";
			for (const AltMask& m : mask_f) {
				std::cerr << m.positive << "\n";
			}
			std::cerr << std::dec << std::endl;
		} else {
			// line looks like: mem[1234] = 5678
			auto preMem = line.find('[');
			auto postMem = line.find(']');
			auto preValue = line.find(" = ");

			uint64_t addr = std::stoull(line.substr(preMem+1, postMem-preMem));
			uint64_t value = std::stoull(line.substr(preValue+3, line.size()));

			addr |= mask_1;
			for (const AltMask& m : mask_f) {
				std::cerr << "Writing " << value << " to addr " << ((addr | m.positive) & m.negative) << std::endl;
				mem.insert_or_assign((addr | m.positive) & m.negative, value);
			}
		}
	}

	uint64_t total{0};
	for (const auto& [addr, value] : mem) {
		std::cerr << "addr : " << addr << " ; value = " << value << std::endl;
		total += value;
	}
	std::cout << total << std::endl;
}
