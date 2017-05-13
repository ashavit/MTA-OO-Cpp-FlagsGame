#include <iostream>
#include "Config.h"
#include "Flags.h"

using namespace std;

int main(int argc, const char* argv[]) {
	if (argc > 1) {
		Flags::configure(argc - 1, argv + 1);
	}
	Flags().run();

	if (DEBUG)
		Flags::test_printMemoryLeakStatus();
	return 0;
}
