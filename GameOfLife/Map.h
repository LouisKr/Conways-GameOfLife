#pragma once
#include "olcPixelGameEngine.h"


class tMap
{
public:
	tMap(int width, int height, bool RandomData);
	~tMap();

	void Draw(olc::PixelGameEngine* Engine, int Zoom, int xOffset, int yOffset);

	int getTile(int x, int y);
	int getTile(int Pos);

	void setTile(int x, int y, int value);
	void setTile(int Pos, int value);

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }

	void setMap(const std::vector<int> &Map) { mMap = Map; }
	const std::vector<int>& getMap() { return mMap; }

private:
	std::vector<int> mMap;

	int mWidth;
	int mHeight;
};