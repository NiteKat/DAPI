#include"ExampleAIModule.h"

DAPI::Game test;
DAPI::PlayerCharacter my_player = test.self();
DAPI::Point target(-1, -1);
bool did_something;

extern "C" __declspec(dllexport) void onFrame()
{
	while (my_player.statPoints())
	{
		my_player.increaseStrength();
	}
	if (my_player.mode() == DAPI::player_mode::PLAYER_MODE_STANDING)
	{
		did_something = false;
		if (my_player.dLevel() == 0)
		{
			if (target.x == -1)
			{
				std::vector<DAPI::Trigger> targets = test.activeTriggers();
				if (targets.size() != 0)
				{
					for (auto &ptarget : targets)
					{
						if (ptarget.getType() == DAPI::interface_mode::WM_DIABNEXTLVL)
						{
							target.x = ptarget.x();
							target.y = ptarget.y();
						}
					}
				}
			}
			else
			{
				int tiles = abs(target.x - my_player.worldX()) + abs(target.y - my_player.worldY());
				DAPI::Point temp_target(target.x, target.y);
				while (tiles > 24)
				{
					temp_target.x += (int)floor((my_player.worldX() - temp_target.x)/ 2);
					temp_target.y += (int)floor((my_player.worldY() - temp_target.y) / 2);
					tiles = abs(temp_target.x - my_player.worldX()) + abs(temp_target.y - my_player.worldY());
				}
				if (temp_target.isClear())
					my_player.walkToXY(temp_target.x, temp_target.y);
				else
					my_player.walkToXY(my_player.worldX(), my_player.worldY() - 1);
			}
		}
		else
		{
			std::vector<DAPI::Monster> monsters = test.liveMonsters();
			std::vector<DAPI::Object> objects = test.validObjects();
			std::vector<DAPI::Door> doors = test.doors();
			if (monsters.size() != 0)
			{
				DAPI::Monster closest_monster;
				DAPI::Door closest_door;
				int closest_tiles = 0;
				for (auto &monster : monsters)
				{
					if (closest_tiles == 0)
					{
						closest_monster = monster;
						closest_tiles = abs(monster.x() - my_player.worldX()) + abs(monster.y() - my_player.worldY());
					}
					else
					{
						int difference = abs(monster.x() - my_player.worldX()) + abs(monster.y() - my_player.worldY());
						if (difference < closest_tiles)
						{
							closest_monster = monster;
							closest_tiles = difference;
						}
					}
				}
				if (closest_tiles < 24 && closest_tiles > 0)
				{
					my_player.attack(closest_monster);
					if (my_player.walkPath()[0] != (char)-1)
						did_something = true;
				}
				if (objects.size() != 0 && !did_something)
				{
					int closest_tiles = 0;
					DAPI::Object closest_object;
					for (auto &object : objects)
					{
						if (closest_tiles == 0)
						{
							closest_object = object;
							closest_tiles = abs(object.x() - my_player.worldX()) + abs(object.y() - my_player.worldY());
						}
						else
						{
							int difference = abs(object.x() - my_player.worldX()) + abs(object.y() - my_player.worldY());
							if (difference < closest_tiles)
							{
								closest_object = object;
								closest_tiles = difference;
							}
						}
					}
					if (closest_tiles < 24 && closest_tiles > 0)
					{
						my_player.operateObject(closest_object);
					}
				}
			}
		}
	}
}