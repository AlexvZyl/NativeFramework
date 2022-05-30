#pragma once

#include "FreeList.h"

// The memory in the Array that is not being used is used as the entries for the Linked List.
// The Double Ended FreeList allows searching forward in reverse.
//
//	-------------------------------------------------------------------------
//	|	    	4		    |			4			|			4			|
//	|	     [Size]		    |	   [Next slot]		|	   [Prev slot]		|
//	-------------------------------------------------------------------------
//  
// Size: The size of the current slot in amount of element.
// Next Slot: The index of the slot that sits after the current one.
// Prev Slot: The index of the slot that sits before the current slot.

#define DEFREELIST_MINIMUM_ELEMENT_SIZE 12  //  Bytes.

template <typename T>
class FreeList : public FreeListBase<T>
{
	// Constructors.
	FreeList() : FreeListBase<T>(DEFREELIST_MINIMUM_ELEMENT_SIZE)
	{
		// Ensure element size is large enough.
        // The FreeList element must be larger than or equal to 12 bytes.
		static_assert(sizeof(T) >= DEFREELIST_MINIMUM_ELEMENT_SIZE); 
	}
	FreeList(int capacity)
		: FreeList()
	{
		queryResize(capacity);
	}
	FreeList(int capacity, int capacityIncrements)
		: FreeList()
	{
		setCapacityIncrements(capacityIncrements);
		queryResize(capacity);
	}

	
};