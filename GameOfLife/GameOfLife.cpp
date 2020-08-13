#include "GameOfLife.h"



tGameOfLife::tGameOfLife(std::shared_ptr<tMap> Map)
	:mMap(Map),
	newMap(tMap(mMap->getWidth(), mMap->getHeight(), false)),
	mRunning(false)
{}

void tGameOfLife::Clear(std::shared_ptr<tMap> Map)
{
	mMap = Map;
	newMap = tMap(mMap->getWidth(), mMap->getHeight(), false);
}

int tGameOfLife::getNeighbours(int x, int y)
{
	int Neighbours = 0;

	if (mMap->getTile(x - 1, y - 1) == 1) //top left										
		Neighbours++;														
	if (mMap->getTile(x, y - 1) == 1) //top							
		Neighbours++;														
	if (mMap->getTile(x + 1, y - 1) == 1) //top right
		Neighbours++;
	if (mMap->getTile(x + 1, y) == 1) //right
		Neighbours++;
	if (mMap->getTile(x + 1, y + 1) == 1) //Bottom right
		Neighbours++;
	if (mMap->getTile(x, y + 1) == 1) //Bottom 
		Neighbours++;
	if (mMap->getTile(x - 1, y + 1) == 1) //Bottom left
		Neighbours++;
	if (mMap->getTile(x - 1, y) == 1) //Left
		Neighbours++;

	return Neighbours;
}

int tGameOfLife::newStatus(int x, int y)
{
	//living Neighbours
	int Neighbours = getNeighbours(x, y);

	if (mMap->getTile(x, y) == 0) //if dead
	{
		if (Neighbours == 3)
			return 1;
		else 
			return 0;
	}
	else if (mMap->getTile(x, y) == 1) //if alive
	{
		if (Neighbours < 2)
			return 0;
		else if (Neighbours == 2 or Neighbours == 3)
			return 1;
		else if (Neighbours > 3)
			return 0;
	}
}

void tGameOfLife::Calculate(int xOffset, int yOffset, int width, int height)
{
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			if (newStatus(x + xOffset, y + yOffset) == 0)
				newMap.setTile(x + xOffset, y + yOffset, 0);
			else if (newStatus(x + xOffset, y + yOffset) == 1)
				newMap.setTile(x + xOffset, y + yOffset, 1);
		}
	}
}

void tGameOfLife::Tick()
{
	if (mRunning)
	{
		if (mMap != nullptr)
		{
			const auto processor_count = std::thread::hardware_concurrency();

			std::vector<std::thread> Threads;
			int thread_count = processor_count * 2; 

			//for (size_t x = 0; x < thread_count / 2; x++)
			//{
			//	for (size_t y = 0; y < thread_count / 2; y++)
			//	{
			//		Threads.push_back(std::thread(&tGameOfLife::Calculate, this, x * (mMap->getWidth() / thread_count), y * (mMap->getHeight() / thread_count), mMap->getWidth() / thread_count, mMap->getHeight() / thread_count));
			//	}
			//}

			for (size_t i = 0; i < thread_count; i++)
			{
				Threads.push_back(std::thread(&tGameOfLife::Calculate, this, i * (mMap->getWidth() / thread_count), 0, mMap->getWidth() / thread_count, mMap->getHeight()));
			}

			for (size_t i = 0; i < Threads.size(); i++)
			{
				Threads[i].join();
			}
			mMap->setMap(newMap.getMap());
		}
	}
}