#pragma once

namespace DAPI
{
	struct AIModule
	{
		AIModule();
		virtual ~AIModule();

		virtual void onFrame() = 0;
	};
}