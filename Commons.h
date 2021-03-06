#pragma once

#include <string>

// File name consts
const std::string RANDOM_FILE_NAME = "random_";
const std::string BOARD_FILE_EXTENSION = "gboard";
const std::string PLAYER_A_FILE_EXTENSION = "moves-a_small";
const std::string PLAYER_B_FILE_EXTENSION = "moves-b_small";

// Board item marks
const char BOARD_MARK_FOREST = 'T';
const char BOARD_MARK_SEA = 'S';
const char BOARD_MARK_FLAG_A = 'A';
const char BOARD_MARK_FLAG_B = 'B';

enum ShipType
{
	INVALID = 0,
	SHIP1 = 1,
	SHIP2,
	SHIP3,
	SHIP7 = 7,
	SHIP8,
	SHIP9
};

enum class Direction
{
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	INVALID
};
