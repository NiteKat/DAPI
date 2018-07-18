#pragma once

namespace DAPI
{
	struct Point
	{
		bool operator!=(const Point& other) {
			return this->x != other.x || this->y != other.y;
		}
		int x;
		int y;
	};
}