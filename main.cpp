#include <iostream>
#include "Config.h"
#include "Flags.h"

using namespace std;

int main(int argc, const char* argv[]) {
	Flags flags;
	flags.configure(argc - 1, argv + 1);
	flags.run();

	if (DEBUG)
		Flags::test_printMemoryLeakStatus();
	return 0;
}
