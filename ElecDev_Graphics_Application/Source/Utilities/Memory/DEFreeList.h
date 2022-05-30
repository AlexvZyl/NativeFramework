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
		queryResize(capacity);
	}
	FreeList(int capacity, int capacityIncrements)
		: FreeList()
	{
		setCapacityIncrements(capacityIncrements);
		queryResize(capacity);
	}

private:

	inline virtual void mergeSlots(int firstSlot, int secondSlot) override
	{
		increaseSlotSize(firstSlot, getSlotSize(secondSlot));
		setNextSlot(firstSlot, getNextSlot(secondSlot));
		if (hasNextSlot(secondSlot)) setPrevSlot(getNextSlot(secondSlot), firstSlot);
		if (m_lastFreeSlot == secondSlot) m_lastFreeSlot = firstSlot;
	}

	inline virtual void connectSlots(int firstSlot, int secondSlot) override			
	{
		setNextSlot(firstSlot, secondSlot); 
		setPrevSlot(secondSlot, firstSlot); 
	}	

	inline virtual int commitSlotFirstFit(int slotSize = 1) override
	{
		// Find an open slot that is large enough, starting from the first free one.
		// If there is no valid first slot, skip the initial search and resize.
		int slotIndex = m_firstFreeSlot;
		if (slotIsValid(slotIndex))
		{
			int currentSlotSize = getSlotSize(slotIndex);
            // Look for a large enough slot.
			while (currentSlotSize < slotSize)
			{
				// Get next slot and check if is valid.
				slotIndex = getNextSlot(slotIndex);
				if (!slotIsValid(slotIndex)) 
				{
					// Did not find a valid slot.  
					// Try to resize and return the last slot.
					if(resizeToFitElement(slotSize)) 
					{
						commitSlot(getPrevSlot(m_lastFreeSlot), m_lastFreeSlot, -1);
						return m_lastFreeSlot;
					}
						// Slot find and resize both failed.
					else assert(false);
				}
				currentSlotSize = getSlotSize(slotIndex);
			}
		}
		// Found a valid slot.
		commitSlot(getPrevSlot(slotIndex), slotIndex, getNextSlot(slotIndex));
		return slotIndex;
	}

	// Get the data describing the slot.
	// { Size, Next Slot, Prev Slot }
	inline std::tuple<int, int, int> getSlotData(int slotIndex)													
	{																											
		int* data = getSlotDataPtr(slotIndex);
		return { data[0], data[1], data[2] };
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
		if (firstFreeSlotValid())
		{
			// Calculate distances.
			int distanceFromFirst = slotIndex - m_firstFreeSlot;
			int distanceFromEnd = m_lastFreeSlot - slotIndex;

			// Left of first slot.
			if (distanceFromFirst < 0) rightSlot = m_firstFreeSlot;

			// Right of last slot.
			else if (distanceFromEnd < 0) leftSlot = m_lastFreeSlot;

			// First slot is closest.
			else if(distanceFromFirst <= distanceFromEnd)
			{
				int referenceSlot = searchForward(m_firstFreeSlot, slotIndex);
				leftSlot = referenceSlot;
				rightSlot = getNextSlot(referenceSlot);
			}
			// Last slot is closest.
			else 
			{
				int referenceSlot = searchReverse(m_lastFreeSlot, slotIndex);
				rightSlot = referenceSlot;
				leftSlot = getPrevSlot(referenceSlot);
			}
		}

		// Return the slots.
		return { leftSlot, rightSlot };
	}
};