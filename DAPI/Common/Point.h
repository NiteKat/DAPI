#pragma once

namespace DAPI
{
	struct Point
	{
		Point() { x = y = 0; }
		Point(int nx, int ny) { x = nx; y = ny; }
		bool isClear() {
			auto PosOkPlayer = reinterpret_cast<bool(*)(int pnum, int px, int py)>(0x44FDBA);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			return PosOkPlayer(*myplr, x, y);
		}
		int x;
		int y;
	};
}