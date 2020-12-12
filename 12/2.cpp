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

class WaypointDistance : public Distance
{
public:
	WaypointDistance(int ns, int we) :
	       Distance{ns, we}
	{}

	void addDegrees(int d) {
		int newNS, newWE;
		if (d == 90 || d == -270) {
			newNS = -easting;
			newWE = northing;
		} else if (d == 180 || d == -180) {
			newNS = -northing;
			newWE = -easting;
		} else if (d == -90 || d == 270) {
			newNS = easting;
			newWE = -northing;
		}
		northing = newNS;
		easting = newWE;
	}
};

int main()
{
	Distance ship;
	WaypointDistance waypoint{1, 10};

	std::cout << waypoint.northing << " " << waypoint.easting << " | "
		  << ship.northing << " " << ship.easting
		  << std::endl;

	std::ifstream input{"input"};
	for (;;) {
		char movement;
		int amount;
		input >> movement >> amount;
		if (!input)
			break;

		switch (movement) {
			case 'N':
				waypoint.northing += amount;
				break;
			case 'S':
				waypoint.northing -= amount;
				break;
			case 'E':
				waypoint.easting += amount;
				break;
			case 'W':
				waypoint.easting -= amount;
				break;
			case 'L':
				waypoint.addDegrees(-amount);
				break;
			case 'R':
				waypoint.addDegrees(amount);
				break;
			case 'F':
				ship.northing += waypoint.northing * amount;
				ship.easting += waypoint.easting * amount;
				break;
		}
		std::cout << movement << amount << std::endl;
		std::cout << waypoint.northing << " " << waypoint.easting << " | "
			  << ship.northing << " " << ship.easting
			  << std::endl;
	}

	std::cout << ship() << std::endl;
}
