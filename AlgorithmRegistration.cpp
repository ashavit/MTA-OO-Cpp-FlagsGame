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

AbstractPlayer * AlgorithmRegistration::getPlayerByFactoryIndex(const int ind)
{
	// Make sure index does not exceed size
	int index = ind % factorySize();
	auto id_func = playerFactory.begin();
	for (size_t i = 0; i < index; i++)
		++id_func;

	auto& func = id_func->second;
	return func();
}

