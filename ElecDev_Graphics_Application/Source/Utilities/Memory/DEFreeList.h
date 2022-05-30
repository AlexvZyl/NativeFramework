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

#define DE_FREELIST_MINIMUM_ELEMENT_SIZE 12  //  Bytes.

template <typename T>
class FreeList : public FreeListBase<T>
{
public:

	// Constructors.
	FreeList() : FreeListBase<T>(DE_FREELIST_MINIMUM_ELEMENT_SIZE)
	{
		// Ensure element size is large enough.
        // The FreeList element must be larger than or equal to 12 bytes.
		static_assert(sizeof(T) >= DE_FREELIST_MINIMUM_ELEMENT_SIZE); 
	}
	FreeList(int capacity)
		: FreeList()
	{
		this->queryResize(capacity);
	}
	FreeList(int capacity, int capacityIncrements)
		: FreeList()
	{
		this->setCapacityIncrements(capacityIncrements);
		this->queryResize(capacity);
	}

private:

	inline virtual void mergeSlots(int firstSlot, int secondSlot) override
	{
		this->increaseSlotSize(firstSlot, this->getSlotSize(secondSlot));
		this->setNextSlot(firstSlot, this->getNextSlot(secondSlot));
		if (this->hasNextSlot(secondSlot)) this->setPrevSlot(this->getNextSlot(secondSlot), firstSlot);
		if (this->m_lastFreeSlot == secondSlot) this->m_lastFreeSlot = firstSlot;
	}

	inline virtual void connectSlots(int firstSlot, int secondSlot) override			
	{
		this->setNextSlot(firstSlot, secondSlot); 
		this->setPrevSlot(secondSlot, firstSlot); 
	}	

	inline std::tuple<int, int> findAdjacentSlots(int slotIndex) override											
	{
		// If not slots are valid these defaults will be returned.
		int leftSlot = -1;
		int rightSlot = -1;

		// If one of the slots are valid, both are valid, since
		// both indexes can point to the same slot.
		// Else there are no valid slots and we do not  need to change the values.
		if (this->firstFreeSlotValid())
		{
			// Calculate distances.
			int distanceFromFirst = slotIndex - this->m_firstFreeSlot;
			int distanceFromEnd = this->m_lastFreeSlot - slotIndex;

			// Left of first slot.
			if (distanceFromFirst < 0) rightSlot = this->m_firstFreeSlot;

			// Right of last slot.
			else if (distanceFromEnd < 0) leftSlot = this->m_lastFreeSlot;

			// First slot is closest.
			else if(distanceFromFirst <= distanceFromEnd)
			{
				leftSlot = this->searchForward(this->m_firstFreeSlot, slotIndex);
				rightSlot = this->getNextSlot(leftSlot);
			}
			// Last slot is closest.
			else 
			{
				rightSlot = this->searchReverse(this->m_lastFreeSlot, slotIndex);
				leftSlot = this->getPrevSlot(rightSlot);
			}
		}

		// Return the slots.
		return { leftSlot, rightSlot };
	}
};