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
	
	// These functions are overridden so that larger functions can be implemented in the the parent, 
	// otherwise two relatively large functions (with only one line difference) has to be maintained in
	// two different plaves.
	inline virtual void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot) override {  setSlotData(slotIndex, slotSize, nextSlot); }
	inline virtual void setPrevSlot(int slotIndex, int prevSlot) override { }
	inline virtual int getPrevSlot(int slotIndex) const override { return 0; }

	inline virtual void connectSlots(int firstSlot , int secondSlot) override
	{
		setNextSlot(firstSlot, secondSlot);		
	}

	inline void mergeSlots(int firstSlot, int secondSlot) 
	{
		increaseSlotSize(firstSlot, getSlotSize(secondSlot));
		setNextSlot(firstSlot, getNextSlot(secondSlot));
		if (m_lastFreeSlot == secondSlot) m_lastFreeSlot = firstSlot;
	}

	inline int commitSlotFirstFit(int size = 1) override
	{

	}

	// Get the data describing the slot.
	// { Size, Next Slot }
	inline std::tuple<int, int> getSlotData(int slotIndex)
	{
		int data* getSlotDataPtr(slotIndex);
		return { data[0], data[1] };
	}
	
};