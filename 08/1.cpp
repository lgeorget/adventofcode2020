#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <set>

using Program = std::vector<std::pair<std::string, int>>;

int main()
{
	Program p;

	{
		std::ifstream input{"input"};
		while (input) {
			std::string opcode;
			int parameter;
			input >> opcode >> parameter;
			if (input)
				p.emplace_back(std::move(opcode), parameter);
		}
	}

	std::set<int> alreadyExecuted;
	int acc = 0;
	int pc = 0;

	while (alreadyExecuted.count(pc) == 0 && pc < p.size()) {
		Program::value_type& insn = p[pc];
		std::cerr << "Executing " << pc << ": " << p[pc].first << " " << p[pc].second << std::endl;
		alreadyExecuted.insert(pc);
		if (insn.first == "nop") {
			pc++;
		} else if (insn.first == "acc") {
			acc += insn.second;
			pc++;
		} else if (insn.first == "jmp") {
			pc += insn.second;
		}
	}

	std::cout << pc << " (" << p[pc].first << " " << p[pc].second << ") is the first instruction executed twice\n";
	std::cout << acc << std::endl;
}
