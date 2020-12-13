#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <limits>

struct Remainder {
	__int128_t modulo;
	__int128_t divisor;
};

std::pair<__int128_t, __int128_t> bezout(__int128_t a, __int128_t b) {
	__int128_t q;
	__int128_t r = b;
	__int128_t s0 = 1, s1 = 0;
	__int128_t t0 = 0, t1 = 1;

	//std::cout << "Computing the Bezout coefficients of " << a << " and " << b << std::endl;

	while (r > 0) {
		q = a / b;
		r = a - q * b;
		__int128_t s = s0 - q * s1;
		__int128_t t = t0 - q * t1;

		a = b;
		b = r;
		s0 = s1;
		s1 = s;
		t0 = t1;
		t1 = t;

	//	std::cout << "{" << s0 <<  "," << t0 << "}" << std::endl;
	}

	return {s0, t0};
}

int main()
{
	std::ifstream input{"input"};
	std::vector<Remainder> equations;
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::string entry;
	__int128_t i=0;
	while (std::getline(input, entry, ',')) {
		if (entry != "x") {
			__int128_t d = std::stoi(entry);
			equations.push_back(Remainder{-i, d});
		}
		i++;
	}


	/*for (auto& p : equations) {
		std::cout << "(" << p.divisor << "," << p.modulo << ")" << std::endl;
	}*/

	Remainder r0 = equations[0];
	for (int i=1 ; i<equations.size() ; i++) {
		const Remainder& r1 = equations[i];

		std::pair<__int128_t, __int128_t> p = bezout(r0.divisor, r1.divisor);
		__int128_t solution = r0.divisor * p.first * r1.modulo + r1.divisor * p.second * r0.modulo;
		/*std::cout << "r0.divisor * p.first * r1.modulo + r1.divisor * p.second * r0.modulo = "
			  << r0.divisor << "*" << p.first << "*" << r1.modulo << " + " << r1.divisor << "*" << p.second << "*" << r0.modulo << " = "
			  << solution << std::endl;*/

		r0.divisor = r0.divisor * r1.divisor;
		solution %= r0.divisor;
		if (solution < 0)
			solution += r0.divisor;
		r0.modulo = solution;
		//std::cout << "solution: " << solution << std::endl;
	}

	std::cout << static_cast<uint64_t>(r0.modulo) << std::endl;
}

