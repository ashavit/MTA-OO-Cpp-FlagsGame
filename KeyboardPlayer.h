#pragma once

#include "Player.h"

class KeyboardPlayer : 	public Player
{
	enum keyOptions
	{
		_SHIP1,
		_SHIP2,
		_SHIP3,
		_UPWARDS,
		_DOWN,
		_LEFT,
		_RIGHT,
		_STOP,
		_keyOptionsSize
	};

	char controlKeys[_keyOptionsSize];

	void setKeys(const char* keys);

public:
	KeyboardPlayer();
	virtual ~KeyboardPlayer();
	KeyboardPlayer(KeyboardPlayer const&) = delete;
	void operator=(KeyboardPlayer const&) = delete;

	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	virtual void setPlayer(int player) override;

	void notifyKeyHit(char ch, unsigned long timeStamp);
};

