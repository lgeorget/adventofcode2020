#include <iostream>
#include <fstream>

struct Distance
{
	int northing = 0;
	int easting = 0;

	int operator()() {
		return std::abs(northing) + std::abs(easting);
	}
};

class Direction
{
private:
	int coeff_ns;
	int coeff_we;

	int degrees = 0;

public:
	Direction(int angle) {
		addDegrees(angle);
	}

	int ns() const { return coeff_ns; }
	int we() const { return coeff_we; }

	void addDegrees(int d) {
		degrees = (degrees + d) % 360;
		if (degrees == 0) {
			coeff_ns = 1;
			coeff_we = 0;
		} else if (degrees == 90 || degrees == -270) {
			coeff_ns = 0;
			coeff_we = 1;
		} else if (degrees == 180 || degrees == -180) {
			coeff_ns = -1;
			coeff_we = 0;
		} else {
			coeff_ns = 0;
			coeff_we = -1;
		}
	}
};

int main()
{
	Direction barring{90};
	Distance distance;

	std::ifstream input{"input"};
	for (;;) {
		char movement;
		int amount;
		input >> movement >> amount;
		if (!input)
			break;

		switch (movement) {
			case 'N':
				distance.northing += amount;
				break;
			case 'S':
				distance.northing -= amount;
				break;
			case 'E':
				distance.easting += amount;
				break;
			case 'W':
				distance.easting -= amount;
				break;
			case 'L':
				barring.addDegrees(-amount);
				break;
			case 'R':
				barring.addDegrees(amount);
				break;
			case 'F':
				distance.northing += barring.ns() * amount;
				distance.easting += barring.we() * amount;
				break;
		}
		std::cout << movement << amount << std::endl;
		std::cout << distance.northing << " " << distance.easting << " | "
			  << barring.ns() << " " << barring.we()
			  << std::endl;
	}

	std::cout << distance() << std::endl;
}
