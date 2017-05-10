#pragma once

#include <stdio.h>
#include <vector>
#include <list>

#define RANDOM_FILE_NAME "random_"
#define BOARD_FILE_EXTENSION ".gboard"
#define PLAYER_A_FILE_EXTENSION ".moves-a_small"
#define PLAYER_B_FILE_EXTENSION ".moves-b_small"

class FileManager
{
	FileManager() {};
	~FileManager() {};

	std::string regulatedPath;
	std::list<std::string> boardFiles;

	void regulatePath(const std::string path);
	void parseFileNames(std::string& data);
	int filesLeft = 0;

public:

	static FileManager& sharedInstance();

	FileManager(FileManager const&) = delete;
	void operator=(FileManager const&) = delete;

	void loadAvailableFiles(const std::string& path);
	const std::string nextFileName();
	bool hasMoreBoards();

	const std::string fileNameWithPath(const std::string fileName);
};

