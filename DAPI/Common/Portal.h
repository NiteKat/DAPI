#pragma once
#include"Structs.h"

namespace DAPI {
	struct Portal {

		Portal(PortalStruct* new_portal) { my_portal = new_portal; }

		Portal() { my_portal = nullptr; }

		//Returns the index at which this portal is stored in the Portal array.
		int id() {
			auto portal = reinterpret_cast<PortalStruct(*)[4]>(0x69BC98);
			for (int i = 0; i < 4; i++) {
				if (&((*portal)[i]) == my_portal)
					return i;
			}
			return -1;
		}

		//Returns true if this portal is open.
		bool open() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (my_portal && (my_portal->level == (*player)[*myplr].plrlevel || (*player)[*myplr].plrlevel == 0))
				return static_cast<bool>(my_portal->open);
			else
				return false;
		}
		
		//Returns the x value of the tile this portal is open on within the dungeon.
		//Portal coordinates in town are set based on the index, and thus are not
		//stored within the Portal.
		int x() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (my_portal && my_portal->level == (*player)[*myplr].plrlevel && my_portal->open)
				return my_portal->x;
			else
				return -1;
		}

		//Returns the y value of the tile this portal is open on within the dungeon.
		//Portal coordinates in town are set based on the index, and thus are not
		//stored within the Portal.
		int y() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (my_portal && my_portal->level == (*player)[*myplr].plrlevel && my_portal->open)
				return my_portal->y;
			else
				return -1;
		}

	private:
		PortalStruct* my_portal;
	};
}