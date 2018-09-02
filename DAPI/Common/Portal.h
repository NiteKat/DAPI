#pragma once
#include"Structs.h"

namespace DAPI {
	struct Portal {

		Portal(PortalStruct* new_portal) { my_portal = new_portal; }

		Portal() { my_portal = nullptr; }

		bool open() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (my_portal && (my_portal->level == (*player)[*myplr].plrlevel || (*player)[*myplr].plrlevel == 0))
				return static_cast<bool>(my_portal->open);
			else
				return false;
		}
		
		int x() {
			auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			auto myplr = reinterpret_cast<int(*)>(0x686444);
			if (my_portal && my_portal->level == (*player)[*myplr].plrlevel && my_portal->open)
				return my_portal->x;
			else
				return -1;
		}

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