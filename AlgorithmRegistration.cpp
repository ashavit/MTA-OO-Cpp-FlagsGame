#include "AlgorithmRegistration.h"

std::map<std::string, std::function<AbstractPlayer*(void)>> AlgorithmRegistration::playerFactory;

AlgorithmRegistration::AlgorithmRegistration(const std::string& id, std::function<AbstractPlayer*(void)> func) {
	playerFactory[id] = func;
}

AbstractPlayer* AlgorithmRegistration::getPlayerForId(const std::string& id) {
	auto id_func = playerFactory.find(id);
	auto& func = id_func->second;
	return func();
}

