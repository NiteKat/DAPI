#pragma once

namespace DAPI
{
	struct Point
	{
		bool operator!=(const Point& other) {
			return this->x != other.x || this->y != other.y;
		}
		bool operator==(const Point& other) const {
			return this->x == other.x && this->y == other.y;
		}
		
		int x;
		int y;
	};

	struct PointCompare {
		bool operator() (const Point& a, const Point& other) const {
			if (a.y < other.y) return true;
			if (a.y < other.y) return false;
			return a.x < other.x;
		}
	};
}