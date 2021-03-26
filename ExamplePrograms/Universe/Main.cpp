#include "Universe.h"


int main() {
	Universe::Universe universe(Universe::Vector2(15000, 10000), 10, 10, 0);

	universe.Start();

	return 0;
}