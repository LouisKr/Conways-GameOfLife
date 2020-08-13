#pragma once
#include "Map.h"
#include <memory>

class tGameOfLife
{
public:
	tGameOfLife(std::shared_ptr<tMap> Map);
	
	void Tick();

	void setRunning(bool Running) { mRunning = Running; }
	bool isRunning() { return mRunning; }

	void Clear(std::shared_ptr<tMap> Map);
private:
	std::shared_ptr<tMap> mMap;
	tMap newMap;

	bool mRunning;

	int newStatus(int x, int y);

	int getNeighbours(int x, int y);

	void Calculate(int x, int y, int width, int height);
};