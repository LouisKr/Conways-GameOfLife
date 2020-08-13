#include "Map.h"

#include <random>

tMap::tMap(int width, int height, bool RandomData)
	:mWidth(width),
	mHeight(height),
	mMap()
{
	mMap.resize(width * height);

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(0, 1);
	if (RandomData)
	{
		for (size_t i = 0; i < mHeight * mWidth; i++)
		{
			mMap[i] = distribution(generator);
		}
	}
}

tMap::~tMap()
{
	//delete[] mMap;
}

void tMap::Draw(olc::PixelGameEngine* Engine, int Zoom, int xOffset, int yOffset)
{
	for (size_t x = 0; x < Engine->ScreenWidth() / Zoom; x++)
	{
		for (size_t y = 0; y < Engine->ScreenHeight() / Zoom; y++)
		{
			if (getTile(x + xOffset, y + yOffset) == 0)
				Engine->FillRect(x * Zoom, y * Zoom, Zoom, Zoom, olc::WHITE);
			else if (getTile(x + xOffset, y + yOffset) == 1)
				Engine->FillRect(x * Zoom, y * Zoom, Zoom, Zoom, olc::BLACK);	
		}
	}
}

int tMap::getTile(int Pos)
{
	return mMap[Pos];
}

int tMap::getTile(int x, int y)
{
	if (x < 0)
		x = mWidth - 1;
	else if (x >= mWidth)
		x = 0;

	if (y < 0)
		y = mHeight - 1;
	else if (y >= mHeight)
		y = 0;

	if ((y * mWidth) + x > mHeight*mWidth)
		return 0;
	return mMap[(y * mWidth) + x];
}

void tMap::setTile(int x, int y, int value)
{
	if ((y * mWidth) + x > mHeight*mWidth)
		return;
	mMap[(y * mWidth) + x] = value;
}

void tMap::setTile(int Pos, int value)
{
	mMap[Pos] = value;
}