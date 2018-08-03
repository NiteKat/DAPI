#pragma once
#include<string>


namespace DAPI
{
	struct DrawRequest
	{
		void createTextRequest(std::string request, bool map_or_screen, int dx, int dy) {
			text = request;
			map_coords = map_or_screen;
			x = dx;
			y = dy;
		}
		std::string text;
		bool map_coords;
		int x;
		int y;
	};
}