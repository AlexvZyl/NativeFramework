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

#define SE_FREELIST_MINIMUM_ELEMENT_SIZE 8  //  Bytes.

template <typename T>
class SEFreeList : public FreeListBase<T>
{
	// Constructors.
	SEFreeList() : FreeListBase<T>(SE_FREELIST_MINIMUM_ELEMENT_SIZE)
	{
		// Ensure element size is large enough.
        // The FreeList element must be larger than or equal to 12 bytes.
		static_assert(sizeof(T) >= SE_FREELIST_MINIMUM_ELEMENT_SIZE); 
	}
	SEFreeList(int capacity)
		: SEFreeList()
	{
		this->queryResize(capacity);
	}
	SEFreeList(int capacity, int capacityIncrements)
		: SEFreeList()
	{
		this->setCapacityIncrements(capacityIncrements);
		this->queryResize(capacity);
	}

private:
	
	// These functions are overridden so that larger functions can be implemented in the the parent, 
	// otherwise two relatively large functions (with only one line difference) has to be maintained in
	// two different plaves.
	inline virtual void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot) override {  this->setSlotData(slotIndex, slotSize, nextSlot); }
	inline virtual void setPrevSlot(int slotIndex, int prevSlot) override { }
	inline virtual int getPrevSlot(int slotIndex) const override { return 0; }

	inline virtual void connectSlots(int firstSlot , int secondSlot) override
	{
		this->setNextSlot(firstSlot, secondSlot);		
	}

	inline void mergeSlots(int firstSlot, int secondSlot) override
	{
		this->increaseSlotSize(firstSlot, this->getSlotSize(secondSlot));
		this->setNextSlot(firstSlot, this->getNextSlot(secondSlot));
		if (this->m_lastFreeSlot == secondSlot) this->m_lastFreeSlot = firstSlot;
	}

	// Find the slots adjacent to the passed slot.  This is used when inserting a slot and having to update the data.			
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

			// Search for the slots.
			else
			{
				leftSlot = this->searchForward(this->m_firstFreeSlot, slotIndex);
				rightSlot = this->getNextSlot(leftSlot);
			}
		}

		// Return the slots.
		return { leftSlot, rightSlot };
	}
};