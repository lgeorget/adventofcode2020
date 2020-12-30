#include <iostream>

constexpr unsigned long long int MAX_ATTEMPTS = 1'000'000'000;

unsigned long long int bruteforcePublicKey(unsigned long long int key)
{
	unsigned long long int subject = 7ull;
	unsigned long long int value = 1ull;
	int turn;
	for (turn = 0 ; value != key && turn < MAX_ATTEMPTS ; ++turn) {
		value = (value * subject) % 20201227;
	}
	return turn;
}

unsigned long long int transform(unsigned long long int subject, int nb)
{
	unsigned long long int value = 1ull;
	int turn;
	for (turn = 0 ; turn < nb ; ++turn) {
		value = (value * subject) % 20201227;
	}
	return value;
}

int main()
{
	unsigned long long int cpk = 13233401;
	unsigned long long int dpk = 6552760;

	unsigned long long int clp = bruteforcePublicKey(cpk);
	unsigned long long int dlp = bruteforcePublicKey(dpk);
	std::cout << transform(dpk, clp) << std::endl;
	// for verification:
	//std::cout << transform(cpk, dlp) << std::endl;
}
