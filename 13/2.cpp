#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <limits>

// Store one modular equation in the form of x ≡ modulo (mod divisor)
struct Remainder {
	__int128_t modulo;
	__int128_t divisor;
};

// Compute the Bezout identity of two numbers a and b, i.e. the number n and m
// such that a * n + b * m = 1
std::pair<__int128_t, __int128_t> bezout(__int128_t a, __int128_t b) {
	// This is a simple (and maybe not very clever) solution of the
	// extended Euclide algorithm.

	// This is the initialization (there are two rows of initialization
	// and we start computing from the third one).
	__int128_t q;
	__int128_t r = b;
	__int128_t s0 = 1, s1 = 0;
	__int128_t t0 = 0, t1 = 1;

	//std::cout << "Computing the Bezout coefficients of " << a << " and " << b << std::endl;

	// When the remainder is equal to 0, the solution is the s and t on the
	// line before.
	while (r > 0) {
		// the integer quotient
		q = a / b;
		// the remainder
		r = a - q * b;
		// the coefficient s
		__int128_t s = s0 - q * s1;
		// the coefficient t
		__int128_t t = t0 - q * t1;

		// Update for the next line
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
	// Open the file
	std::ifstream input{"input"};
	// Prepare the data structure to store the equations
	std::vector<Remainder> equations;
	// Ignore the first line of input
	input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	// Read the entries one by one, ignore the entries equal to "x"
	// The other entries are bus IDs. Their index in the list is the offset
	// of their departure time with respect to the solution timestamp.
	// So, if the bus ID is "d" (d being then also the period in minutes
	// between each one of its departure), its index is "i", and the
	// solution is "x", we know that the bus departs at i minutes after x.
	// In other words, we have x + i ≡ 0 (mod d), or x ≡ -i (mod d).
	std::string entry;
	__int128_t i=0;
	while (std::getline(input, entry, ',')) {
		if (entry != "x") {
			__int128_t d = std::stoi(entry);
			equations.push_back(Remainder{-i, d});
		}
		i++;
	}


	// Optionally print the equation
	/*for (auto& p : equations) {
		std::cout << "(" << p.divisor << "," << p.modulo << ")" << std::endl;
	}*/

	// Left-fold the system of equations (the equation x ≡ 0 (mod 1) is
	// a neutral element in the system since it's trivially satisfied by
	// any x ∈ Q) using the Chinese remainder theorem.
	Remainder solution{0, 1};
	solution = std::accumulate(equations.begin(), equations.end(), solution,
		[](const Remainder& r0, const Remainder& r1) {
			std::pair<__int128_t, __int128_t> p = bezout(r0.divisor, r1.divisor);
			__int128_t newDivisor = r0.divisor * r1.divisor;
			__int128_t solution = r0.divisor * p.first * r1.modulo +
			                      r1.divisor * p.second * r0.modulo;
			/*std::cout << "r0.divisor * p.first * r1.modulo + r1.divisor * p.second * r0.modulo = "
				  << r0.divisor << "*" << p.first << "*" << r1.modulo << " + " << r1.divisor << "*" << p.second << "*" << r0.modulo << " = "
				  << solution << std::endl;*/

			// Any solution modulo newDivisor would do, taking one
			// of the two solutions between -newDivisor and
			// newDivisor keeps calculation as quick as possible
			// and avoid overflowing too quickly.
			return Remainder{solution % newDivisor, newDivisor};
		}
	);

	// Take the smallest positive solution
	if (solution.modulo < 0)
		solution.modulo += solution.divisor;
	// We static_cast to uint64_t because we don't have an output
	// operator for __int28_t and we know the solution is positive now...
	std::cout << static_cast<uint64_t>(solution.modulo) << std::endl;
}

