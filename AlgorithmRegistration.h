#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "AbstractPlayer.h"

class AlgorithmRegistration
{
	static std::map<std::string, std::function<AbstractPlayer*(void)>> playerFactory;

public:
	AlgorithmRegistration(const std::string& id, std::function<AbstractPlayer*(void)> func);
	~AlgorithmRegistration() {};

	static AbstractPlayer* getPlayerForId(const std::string& id);
};

