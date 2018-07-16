#include"ExampleAIModule.h"

DAPI::Game test;
DAPI::PlayerCharacter my_player = test.self();
DAPI::Point target(-1, -1);
std::mt19937 mt = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
int tileVisits[112][112] = { 0 };
int frame_timer = 0;
bool did_something;

extern "C" __declspec(dllexport) void onFrame()
{
	/*DAPI::Item ring = my_player.getEquippedItem(DAPI::equip_slot::HEAD);
	if (ring.isValid())
	{
		my_player.putInCursor(ring);
		char* grid = my_player.getInventoryGrid();
		for (int i = 0; i < 40; i++)
		{
			if (i != 9 && i != 19 && i < 29)
			{
				if (grid[i] == '\0')
				{
					my_player.putCursorItem(i + 7);
					break;
				}
			}
		}
	}*/
	
	for (auto &spell : my_player.getLearnedSpells())
	{
		if (my_player.getRightClickSpell() != spell)
		{
			if (my_player.getSpellLevel(spell) > my_player.getSpellLevel(my_player.getRightClickSpell()))
			{
				my_player.setRightClickSpell(spell);
			}
		}
	}
	if (frame_timer > 0)
		frame_timer--;
	else
	{
		while (my_player.statPoints())
		{
			if (my_player.getClass() == DAPI::_ui_classes::UI_WARRIOR)
			{
				if (my_player.baseDexterity() < 60)
					my_player.increaseDexterity();
				else
					my_player.increaseStrength();
			}
			else if (my_player.getClass() == DAPI::_ui_classes::UI_ROGUE)
				my_player.increaseDexterity();
			else
				my_player.increaseMagic();
		}
		if (my_player.hitPoints() < 20)
		{
			std::vector<DAPI::Item> my_belt = my_player.getBeltItems();
			for (auto item : my_belt)
			{
				if (item.miscId() == DAPI::item_misc_id::IMISC_HEAL ||
					item.miscId() == DAPI::item_misc_id::IMISC_FULLHEAL ||
					item.miscId() == DAPI::item_misc_id::IMISC_REJUV ||
					item.miscId() == DAPI::item_misc_id::IMISC_FULLREJUV)
				{
					my_player.useItem(item);
					break;
				}
			}
		}
		if (my_player.mana() < my_player.getRightClickSpellManaCost())
		{
			std::vector<DAPI::Item> my_belt = my_player.getBeltItems();
			for (auto item : my_belt)
			{
				if (item.miscId() == DAPI::item_misc_id::IMISC_MANA ||
					item.miscId() == DAPI::item_misc_id::IMISC_FULLMANA ||
					item.miscId() == DAPI::item_misc_id::IMISC_REJUV ||
					item.miscId() == DAPI::item_misc_id::IMISC_FULLREJUV)
				{
					my_player.useItem(item);
					break;
				}
			}
		}
		if (my_player.mode() == DAPI::player_mode::PLAYER_MODE_STANDING)
		{
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
						temp_target.x += (int)floor((my_player.worldX() - temp_target.x) / 2);
						temp_target.y += (int)floor((my_player.worldY() - temp_target.y) / 2);
						tiles = abs(temp_target.x - my_player.worldX()) + abs(temp_target.y - my_player.worldY());
					}
					if (temp_target.isClear())
					{
						my_player.walkToXY(temp_target.x, temp_target.y);
						frame_timer = 40;
					}
					else
					{
						my_player.walkToXY(my_player.worldX(), my_player.worldY() - 1);
						frame_timer = 40;
					}
				}
			}
			else
			{
				std::vector<DAPI::Monster> monsters = test.liveMonsters();
				std::vector<DAPI::Object> objects = test.validObjects();
				std::vector<DAPI::Door> doors = test.doors();
				std::vector<DAPI::Item> ground_items = test.groundItems();
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
						if (my_player.getRightClickSpell() != DAPI::spell_id::SPL_NULL &&
							my_player.mana() >= my_player.getRightClickSpellManaCost())
						{
							my_player.castSpell(closest_monster.futurex(), closest_monster.futurey());
							frame_timer = 80;
						}
						else
						{
							my_player.attack(closest_monster);
							DAPI::Item weapon = my_player.getEquippedItem(DAPI::equip_slot::RIGHTHAND);
							if (weapon.type() != DAPI::item_type::ITYPE_BOW)
							{
								if (my_player.walkPath()[0] != (char)-1 || (abs(closest_monster.futurex() - my_player.worldX()) < 2 && abs(closest_monster.futurey() - my_player.worldY()) < 2))
									frame_timer = 40;
							}
							else
								frame_timer = 40;
						}
					}
				}
				if (ground_items.size() != 0 && frame_timer == 0)
				{
					int closest_tiles = 0;
					DAPI::Item closest_item;
					for (auto &item : ground_items)
					{
						if (closest_tiles == 0)
						{
							closest_item = item;
							closest_tiles = abs(item.x() - my_player.worldX()) + abs(item.y() - my_player.worldY());
						}
						else
						{
							int difference = abs(item.x() - my_player.worldX()) + abs(item.y() - my_player.worldY());
							if (difference < closest_tiles)
							{
								closest_item = item;
								closest_tiles = difference;
							}
						}
					}
					if (closest_tiles < 24 && closest_tiles > 0)
					{
						my_player.pickupItem(closest_item);
						frame_timer = 40;
					}
				}
				if (objects.size() != 0 && frame_timer == 0)
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
						frame_timer = 40;
					}
				}
				if (doors.size() != 0 && frame_timer == 0)
				{
					int closest_tiles = 0;
					DAPI::Door closest_door;
					for (auto &door : doors)
					{
						if (closest_tiles == 0 && !door.isOpen())
						{
							closest_door = door;
							closest_tiles = abs(door.x() - my_player.worldX()) + abs(door.y() - my_player.worldY());
						}
						else if (!door.isOpen())
						{
							int difference = abs(door.x() - my_player.worldX()) + abs(door.y() - my_player.worldY());
							if (difference < closest_tiles)
							{
								closest_door = door;
								closest_tiles = difference;
							}
						}
					}
					if (closest_tiles < 24 && closest_tiles > 0)
					{
						my_player.openDoor(closest_door);
						frame_timer = 40;
					}
				}
				if (frame_timer == 0)
				{
					walkWeightedRandomDirection();
					frame_timer = 40;
				}
			}
		}
	}
}

int getRandomInteger(int min, int max)
{
	std::uniform_int_distribution<int> random_number(min, max);
	return random_number(mt);
}

void walkWeightedRandomDirection()
{
	DAPI::Point target;
	DAPI::Point east(my_player.worldX() + 1, my_player.worldY() - 1);
	DAPI::Point north(my_player.worldX() - 1, my_player.worldY() - 1);
	DAPI::Point northeast(my_player.worldX(), my_player.worldY() - 1);
	DAPI::Point northwest(my_player.worldX() - 1, my_player.worldY());
	DAPI::Point south(my_player.worldX() + 1, my_player.worldY() + 1);
	DAPI::Point southeast(my_player.worldX() + 1, my_player.worldY());
	DAPI::Point southwest(my_player.worldX(), my_player.worldY() + 1);
	DAPI::Point west(my_player.worldX() - 1, my_player.worldY() + 1);
	int eastscore = 100 - tileVisits[east.x][east.y];
	int northscore = 100 - tileVisits[north.x][north.y];
	int northeastscore = 100 - tileVisits[northeast.x][northeast.y];
	int northwestscore = 100 - tileVisits[northwest.x][northwest.y];
	int southscore = 100 - tileVisits[south.x][south.y];
	int southeastscore = 100 - tileVisits[southeast.x][southeast.y];
	int southwestscore = 100 - tileVisits[southwest.x][southwest.y];
	int westscore = 100 - tileVisits[west.x][west.y];

	int max_score = eastscore + northscore + northeastscore + northwestscore + southscore + southeastscore + southwestscore + westscore;
	DAPI::direction direction;
	int score = getRandomInteger(1, max_score);
	if (score <= eastscore)
	{
		target.x = my_player.worldX() + 1;
		target.y = my_player.worldY() - 1;
	}
	else if (score <= eastscore + northscore)
	{
		target.x = my_player.worldX() - 1;
		target.y = my_player.worldY() - 1;
	}
	else if (score <= eastscore + northscore + northeastscore)
	{
		target.x = my_player.worldX();
		target.y = my_player.worldY() - 1;
	}
	else if (score <= eastscore + northscore + northeastscore + northwestscore)
	{
		target.x = my_player.worldX() - 1;
		target.y = my_player.worldY();
	}
	else if (score <= eastscore + northscore + northeastscore + northwestscore + southscore)
	{
		target.x = my_player.worldX() + 1;
		target.y = my_player.worldY() + 1;
	}
	else if (score <= eastscore + northscore + northeastscore + northwestscore + southscore + southeastscore)
	{
		target.x = my_player.worldX() + 1;
		target.y = my_player.worldY();
	}
	else if (score <= eastscore + northscore + northeastscore + northwestscore + southscore + southeastscore + southwestscore)
	{
		target.x = my_player.worldX();
		target.y = my_player.worldY() + 1;
	}
	else
	{
		target.x = my_player.worldX() - 1;
		target.y = my_player.worldY() + 1;
	}
	
	my_player.walkToXY(target.x, target.y);
	if (tileVisits[target.x][target.y] < 99)
		tileVisits[target.x][target.y] += 1;
	
}