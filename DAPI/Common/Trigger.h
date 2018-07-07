#pragma once
#include"Structs.h"
#include"Enums.h"

namespace DAPI
{
	struct Trigger
	{
		Trigger(TriggerStruct* new_trigger) { my_trigger = new_trigger; }
		interface_mode getType() { return (interface_mode)my_trigger->_tmsg; }
		int x() { return my_trigger->_tx; }
		int y() { return my_trigger->_ty; }
	private:
		TriggerStruct* my_trigger;
	};
}