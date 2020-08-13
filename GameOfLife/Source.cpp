#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Map.h"
#include "GameOfLife.h"

#include <random>

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "GameOfLife";
	}

public:
	int Zoom = 4;

	float xOffset = 0;
	float yOffset = 0;

	int maxTick = 60;
	float TickCount = 0;

	void ZoomIn()
	{
		Zoom++;
	}

	void ZoomOut()
	{
		if (Zoom - 1 > 0)
			Zoom--;
	}

	std::shared_ptr<tMap> Map;

	tGameOfLife* GameOfLife;
	
	olc::vd2d LastMousePos;

	bool OnUserCreate() override
	{
		Map = std::make_shared<tMap>(ScreenWidth() * 2, ScreenHeight() * 2, true);
		GameOfLife = new tGameOfLife(Map);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetMouse(0).bHeld)
		{
			if (GetMouseX() + xOffset < ScreenWidth() * 2  and GetMouseY() + yOffset < ScreenHeight() * 2 and
				GetMouseX() > 0 and GetMouseY() > 0)
				Map->setTile((GetMouseX() / Zoom) + xOffset, (GetMouseY() / Zoom) + yOffset, 1);
		}
		if (GetMouse(1).bHeld)
		{
			if (GetMouseX() + xOffset < ScreenWidth() * 2 and GetMouseY() + yOffset < ScreenHeight() * 2 and
				GetMouseX() > 0 and GetMouseY() > 0)
				Map->setTile((GetMouseX() / Zoom) + xOffset, (GetMouseY() / Zoom) + yOffset, 0);
		}

		if (GetKey(olc::SPACE).bPressed)
		{
			if (GameOfLife->isRunning())
				GameOfLife->setRunning(false);
			else
				GameOfLife->setRunning(true);
		}

		if (GetKey(olc::Key::NP_ADD).bPressed)
		{
			maxTick += 5;
		}

		if (GetKey(olc::Key::NP_SUB).bPressed)
		{
			if (maxTick - 5 > 0)
				maxTick -= 5;
		}

		if (GetKey(olc::Key::CTRL).bHeld)
		{
			if (GetMouseWheel() == 120 or GetMouseWheel() == 240)
			{
				ZoomIn();
				if (GetMouseX() - (ScreenWidth() / 2) > 0)
					xOffset -= ((ScreenWidth() / 2) - GetMouseX()) / Zoom;
				else
					xOffset += (GetMouseX() - (ScreenWidth() / 2)) / Zoom;

				if (GetMouseY() - (ScreenHeight() / 2) > 0)
					yOffset -= ((ScreenHeight() / 2) - GetMouseY()) / Zoom;
				else
					yOffset += (GetMouseY() - (ScreenHeight() / 2)) / Zoom;
			}
			if (GetMouseWheel() == -120 or GetMouseWheel() == -240)
			{
				ZoomOut();
			}
		}

		if (GetKey(olc::RIGHT).bHeld)
		{
			xOffset += 240 * fElapsedTime;
		}

		if (GetKey(olc::LEFT).bHeld)
		{
			xOffset -= 240 * fElapsedTime;
		}

		if (GetKey(olc::R).bPressed)
		{
			GameOfLife->setRunning(false);

			std::vector<int> newMap;
			newMap.resize((ScreenWidth() * 2) * (ScreenHeight() * 2));

			std::random_device device;
			std::mt19937 generator(device());
			std::uniform_int_distribution<int> distribution(0, 1);
			for (size_t i = 0; i < (ScreenWidth() * 2) * (ScreenHeight() * 2); i++)
			{
				newMap[i] = distribution(generator);
			}
			Map->setMap(newMap);
			GameOfLife->Clear(Map);
		}

		if (GetKey(olc::E).bPressed)
		{
			GameOfLife->setRunning(false);

			std::vector<int> newMap;
			newMap.resize((ScreenWidth() * 2) * (ScreenHeight() * 2));
			for (size_t i = 0; i < (ScreenWidth() * 2) * (ScreenHeight() * 2); i++)
			{
				newMap[i] = 0;
			}
			Map->setMap(newMap);
			GameOfLife->Clear(Map);
		}

		if (GetMouse(2).bHeld)
		{
			if (GetMouseX() - LastMousePos.x > 0)
			{
				if (xOffset - (GetMouseX() - LastMousePos.x) / Zoom > 0)
					xOffset -= (GetMouseX() - LastMousePos.x) / Zoom;
			}
			else
			{
				if (xOffset + (LastMousePos.x - GetMouseX()) / Zoom < ScreenWidth())
					xOffset += (LastMousePos.x - GetMouseX()) / Zoom;
			}

			if (GetMouseY() - LastMousePos.y > 0)
			{
				if (yOffset - (GetMouseY() - LastMousePos.y) / Zoom > 0)
					yOffset -= (GetMouseY() - LastMousePos.y) / Zoom;
			}
			else
			{
				if (yOffset + (LastMousePos.y - GetMouseY()) / Zoom < ScreenHeight())
					yOffset += (LastMousePos.y - GetMouseY()) / Zoom;
			}
		}

		TickCount += maxTick * fElapsedTime;
		if (TickCount > 1)
		{
			GameOfLife->Tick();
			TickCount = 0;
		}
		Map->Draw(this, Zoom, xOffset, yOffset);

		DrawString(10, 10, "X" + std::to_string(Zoom), olc::MAGENTA, 3);
		DrawString(70, 18, "Speed: " + std::to_string(maxTick), olc::MAGENTA, 2);


		LastMousePos = olc::vd2d(GetMouseX(), GetMouseY());
		return true;
	}
};

int main()
{
	Game window;
	if (window.Construct(800, 500, 1, 1))
		window.Start();
	return 0;
}