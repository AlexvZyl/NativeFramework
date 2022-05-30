#pragma once

#include "FreeList.h"

// The memory in the Array that is not being used is used as the entries for the Linked List.
// The Single Ended FreeList only supports searching in one direction but has less data to update the
// and supports glm::vec2 and Ind2/UInd2.
//
//	-------------------------------------------------
//	|	    	4		    |			4			|
//	|	     [Size]		    |	   [Next slot]		|
//	-------------------------------------------------
//  
// Size: The size of the current slot in amount of element.
// Next Slot: The index of the slot that sits after the current one.

#define SEFREELIST_MINIMUM_ELEMENT_SIZE 8  //  Bytes.

template <typename T>
class SEFreeList : public FreeListBase<T>
{
	// Constructors.
	SEFreeList() : FreeListBase<T>(SEFREELIST_MINIMUM_ELEMENT_SIZE)
	{
		// Ensure element size is large enough.
        // The FreeList element must be larger than or equal to 12 bytes.
		static_assert(sizeof(T) >= SEFREELIST_MINIMUM_ELEMENT_SIZE); 
	}
	SEFreeList(int capacity)
		: FreeList()
	{
		queryResize(capacity);
	}
	SEFreeList(int capacity, int capacityIncrements)
		: FreeList()
	{
		setCapacityIncrements(capacityIncrements);
		queryResize(capacity);
	}

private:
	
	// This function is overridded so that it can be used in the parent implementation.
	inline virtual void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot) override { setSlotData(slotIndex, slotSize, nextSlot); }
	
};