#include "AlgorithmRegistration.h"

std::map<std::string, std::function<AbstractPlayer*(void)>> AlgorithmRegistration::playerFactory;

AlgorithmRegistration::AlgorithmRegistration(const std::string& id, std::function<AbstractPlayer*(void)> func) {
	playerFactory[id] = func;
}

