#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <set>

struct Insn {
	std::string opcode;
	int param;
};

std::ostream& operator<<(std::ostream& out, const Insn& insn)
{
	out << "[" << insn.opcode << " " << insn.param << "]";
	return out;
}

using Program = std::vector<Insn>;

int main()
{
	Program p;

	// scoping block for input, so that the file is closed as soon as
	// possible
	{
		std::ifstream input{"input"};
		while (input) {
			std::string opcode;
			int parameter;
			input >> opcode >> parameter;
			if (input)
				p.emplace_back(Insn{std::move(opcode), parameter});
		}
	}

	// We must change only one instruction, so only the nop-s and jmp-s
	// found in the normal execution of the program are to be changed
	std::set<int> alreadyExecuted;
	std::vector<int> changeable;
	int acc = 0;
	int pc = 0;
	while (alreadyExecuted.count(pc) == 0 && pc < p.size()) {
		const Insn& insn = p[pc];
		std::cerr << "Executing " << pc << ": " << p[pc] << std::endl;
		alreadyExecuted.insert(pc);
		if (insn.opcode == "nop") {
			changeable.push_back(pc);
			pc++;
		} else if (insn.opcode == "acc") {
			acc += insn.param;
			pc++;
		} else if (insn.opcode == "jmp") {
			changeable.push_back(pc);
			pc += insn.param;
		}
	}
	std::cerr << "Changeable instructions : ";
	for (int i : changeable) {
		std::cerr << i << " " << p[i] << " ; ";
	}
	std::cerr << std::endl;

	// try changing any nop -> jmp or jmp -> nop and execute the program
	for (int i : changeable) {
		alreadyExecuted.clear();
		acc = 0;
		pc = 0;

		std::cerr << "Attempt to modify insn " << i << " " << p[i] << std::endl;
		while (alreadyExecuted.count(pc) == 0 && pc < p.size()) {
			const Insn& insn = p[pc];
			std::cerr << "Executing " << pc << ": " << p[pc] << std::endl;
			alreadyExecuted.insert(pc);

			if (pc == i) {
				if (insn.opcode == "nop") {
					std::cerr << "Changing it to jmp" << std::endl;
					pc += insn.param;
				} else {
					std::cerr << "Changing it to nop" << std::endl;
					pc++;
				}
			} else {
				if (insn.opcode == "nop") {
					pc++;
				} else if (insn.opcode == "acc") {
					acc += insn.param;
					pc++;
				} else if (insn.opcode == "jmp") {
					pc += insn.param;
				}
			}
		}

		// Check if we are at the first instruction after the program
		if (pc == p.size()) {
			std::cerr << "Fixed! " << std::endl;
			std::cout << acc << std::endl;
			return 0;
		} else {
			std::cerr << "That didn't work" << std::endl;
		}
	}
}
