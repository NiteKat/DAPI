#pragma once
#include"Structs.h"
#include"PlayerCharacter.h"

namespace DAPI
{
	struct Game
	{
		PlayerCharacter self()
		{
			static auto player = reinterpret_cast<PlayerStruct(*)[4]>(0x686448);
			static auto myplr = reinterpret_cast<int(*)>(0x686444);
			PlayerCharacter myself(*myplr);
			return myself;
		}

		auto test()
		{
			auto trigs = reinterpret_cast<TriggerStruct(*)[5]>(0x6ABAE0);
			return trigs;
		}

	private:
		

	};

	extern Game Diablo;
}