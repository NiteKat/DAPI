#pragma once
#include"Structs.h"
#include"PlayerCharacter.h"
#include<vector>
#include"Trigger.h"
#include"Object.h"
#include"Monster.h"
#include"Door.h"
#include"Item.h"
#include"Point.h"
#include<queue>
#include"DrawRequest.h"
#include<variant>



namespace DAPI
{
	template<class... Ts> struct Overload : Ts... {using Ts::operator() ...; };
	template<class...Ts> Overload(Ts...)->Overload<Ts...>;

	struct Game
	{
		auto activeTriggers() {
			auto trigflag = reinterpret_cast<int(*)[5]>(0x6ABAC8);
			auto trigs = reinterpret_cast<TriggerStruct(*)[5]>(0x6ABAE0);
			std::vector<Trigger> return_value;
			for (int loop = 0; loop < (*trigflag)[4]; loop++)
			{
				Trigger new_trigger(&(*trigs)[loop]);
				return_value.push_back(new_trigger);
			}
			return return_value;

		}
		
		cursor_id cursor() {
			static auto pcurs = reinterpret_cast<int(*)>(0x4B8CD0);
			return static_cast<cursor_id>(*pcurs);
		}

		auto doors() {
			std::vector<Door> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dObject = reinterpret_cast<char(*)[112][112]>(0x539C48);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dObject)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						switch (static_cast<_object_id>((*object)[index - 1]._otype))
						{
						case _object_id::OBJ_L1LDOOR:
						case _object_id::OBJ_L1RDOOR:
						case _object_id::OBJ_L2LDOOR:
						case _object_id::OBJ_L2RDOOR:
						case _object_id::OBJ_L3LDOOR:
						case _object_id::OBJ_L3RDOOR:
							return_value.push_back(DAPI::Door(&(*object)[index - 1]));
						default:
							break;
						}
					}
				}
			}
			return return_value;
		}

		auto getOnScreenObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dObject = reinterpret_cast<char(*)[112][112]>(0x539C48);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dObject)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						switch (static_cast<_object_id>((*object)[index - 1]._otype))
						{
						case _object_id::OBJ_L1LDOOR:
						case _object_id::OBJ_L1RDOOR:
						case _object_id::OBJ_L2LDOOR:
						case _object_id::OBJ_L2RDOOR:
						case _object_id::OBJ_L3LDOOR:
						case _object_id::OBJ_L3RDOOR:
							break;
						default:
							DAPI::Object new_object(&(*object)[index - 1]);
							return_value.push_back(new_object);
						}
					}
				}
			}
			return return_value;
		}

		std::vector<DAPI::Point> getOnScreenPoints() {
			auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			std::vector<DAPI::Point> return_value;
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					DAPI::Point new_point{ (*player)[*myplr].WorldX + dx + cur_cell, (*player)[*myplr].WorldY + dy + cur_cell };
					return_value.push_back(new_point);
				}
			}
			return return_value;
		}

		auto groundItems() {
			std::vector<Item> return_value;
			auto item = reinterpret_cast<ItemStruct(*)[127]>(0x635A28);
			auto dItem = reinterpret_cast<char(*)[112][112]>(0x5C9A10);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dItem)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						Item new_item(&(*item)[index - 1]);
						return_value.push_back(new_item);
					}
				}
			}
			return return_value;
		}

		bool isInventoryOpen() {
			static auto invflag = reinterpret_cast<int*>(0x634CB8);
			return invflag;
		}

		bool isSolid(DAPI::Point point) {
			auto dPiece = reinterpret_cast<int(*)[112][112]>(0x5A5BD8);
			static auto nSolidTable = reinterpret_cast<char(*)[2049]>(0x5BB2F0);
			if (!(*dPiece)[point.x][point.y] || (*nSolidTable)[(*dPiece)[point.x][point.y]])
				return true;
			else
				return false;
		}

		auto liveMonsters() {
			std::vector<Monster> return_value;
			auto monster = reinterpret_cast<MonsterStruct(*)[200]>(0x64D330);
			static auto dMonster = reinterpret_cast<int(*)[112][112]>(0x52D208);
			static auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			int dx = -11;
			int dy = 0;
			int cells = 12;
			for (int i = 0; i < 21; i++)
			{
				bool cells_are_11 = cells == 11;
				cells += cells_are_11 - !cells_are_11;
				dy -= cells_are_11;
				dx += !cells_are_11;
				for (int cur_cell = 0; cur_cell < cells; cur_cell++)
				{
					int index = (*dMonster)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell];
					if (0 < index)
					{
						if ((*dFlags)[(*player)[*myplr].WorldX + dx + cur_cell][(*player)[*myplr].WorldY + dy + cur_cell] & 0x40 &&
							(*monster)[index - 1]._mhitpoints > 0)
						{
							DAPI::Monster new_monster(&(*monster)[index - 1]);
							return_value.push_back(new_monster);
						}
					}
				}
			}
			return return_value;
		}

		PlayerCharacter self() {
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			PlayerCharacter myself(*myplr);
			return myself;
		}

		void toggleInventory() {
			static auto invflag = reinterpret_cast<int*>(0x634CB8);
			if (invflag)
				*invflag = 0;
			else
				*invflag = 1;
		}

		auto validObjects() {
			std::vector<Object> return_value;
			auto object = reinterpret_cast<ObjectStruct(*)[127]>(0x679C38);
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			for (int i = 0; i < 127; i++)
			{
				bool add_object = false;
				switch (static_cast<_object_id>((*object)[i]._otype))
				{
				case _object_id::OBJ_BARREL:
				case _object_id::OBJ_BARRELEX:
					if ((*object)[i]._oSolidFlag)
						add_object = true;
					break;
				case _object_id::OBJ_CHEST1:
				case _object_id::OBJ_TCHEST1:
				case _object_id::OBJ_CHEST2:
				case _object_id::OBJ_TCHEST2:
				case _object_id::OBJ_CHEST3:
				case _object_id::OBJ_TCHEST3:
				case _object_id::OBJ_SARC:
					if ((*object)[i]._oSelFlag)
						add_object = true;
					break;
				default:
					break;
				}
				if (add_object)
				{
					int ox = (*object)[i]._ox;
					int oy = (*object)[i]._oy;
					if ((*dFlags)[ox][oy] & 0x40)
					{
						Object new_object(&(*object)[i]);
						return_value.push_back(new_object);
					}
				}
			}
			return return_value;
		}

		auto visiblePoints() {
			auto dFlags = reinterpret_cast<char(*)[112][112]>(0x5C6910);
			std::vector<Point> return_value;
			for (int x = 0; x < 112; x++)
			{
				for (int y = 0; y < 112; y++)
				{
					if ((*dFlags)[x][y] & 0x40)
						return_value.push_back(Point{ x, y });
				}
			}
			return return_value;
		}

		template<typename T>
		void drawTextToScreen(int x, int y, T &&string) {
			draw_queue.emplace_back(StringDrawScreen{ x, y, std::forward<T>(string) });
		}

		void onDraw() {
			static auto PrintGameStr = reinterpret_cast<void(__fastcall *)(int x, int y, char *str, int color)>(0x405681);
			static auto dx_lock_mutex = reinterpret_cast<void(__cdecl *)()>(0x41569A);
			static auto dx_unlock_mutex = reinterpret_cast<void(__cdecl *)()>(0x415725);
			auto clean = [&] {
				struct Cleanup {
					Cleanup(drawQueueType &queue) : toClean{ queue } {}
					~Cleanup() { toClean.clear(); }
					drawQueueType &toClean;
				};

				return Cleanup(draw_queue);
			}();

			dx_lock_mutex();
			for (auto &drawable : draw_queue) {
				std::visit(Overload{
					[](StringDrawScreen &text) {
					PrintGameStr(text.x, text.y, text.s.data(), 3);
					},
					[](StringDrawMap &text) {
					//Point print_location {tile}
					}
				}, drawable);
			}
			dx_unlock_mutex();
		}

	private:
		struct StringDrawScreen {
			int x, y;
			std::string s;
		};

		struct StringDrawMap {
			int x, y;
			std::string s;
		};

		using DrawObject = std::variant<StringDrawScreen,
			StringDrawMap
			// add more here as needed
		>;
		using drawQueueType = std::vector<DrawObject>;
		drawQueueType draw_queue;
		
	};

	
}