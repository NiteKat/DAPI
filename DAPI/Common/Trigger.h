#pragma once
#include"Structs.h"
#include"Enums.h"

namespace DAPI
{
	struct Trigger
	{
		Trigger() { my_trigger = nullptr; }
		Trigger(TriggerStruct* new_trigger) { my_trigger = new_trigger; }
		interface_mode getType() { return (interface_mode)my_trigger->_tmsg; }

		bool operator== (const Trigger& other) const {
			return this->my_trigger->_tx == other.my_trigger->_tx && this->my_trigger->_ty == other.my_trigger->_ty && this->my_trigger->_tlvl == other.my_trigger->_tlvl && this->my_trigger->_tmsg == other.my_trigger->_tmsg;
		}

		int x() { return my_trigger->_tx; }
		int y() { return my_trigger->_ty; }
	private:
		TriggerStruct* my_trigger;
	};
}