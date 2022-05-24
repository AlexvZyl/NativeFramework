#pragma once

#include "Utilities/Logger/Logger.h"

// When data has been freed the first 64 bits of the slot is used to store the
// value of the next open slot as well as the size of the currently open slot.
// This means FreeLists cannot use glm::vec2... :(
#define MINIMUM_FREELIST_ELEMENT_SIZE 12 // Bytes.

template<typename T>
class FreeList 
{

public:

	// Constructors.
	FreeList() 
	{
		// Ensure element size is large enough.
		static_assert(sizeof(T) >= MINIMUM_FREELIST_ELEMENT_SIZE, "The FreeList element must be larger than or equal to 12 bytes.");
		m_sizeOfElement = sizeof(T);
	}
	FreeList(int newCapacity) 
		: FreeList()
	{
		allocate(newCapacity);
	}
	FreeList(int size, int resizeIncrements) 
		: FreeList(size)
	{
		setResizeIncrements(resizeIncrements);
	}

	// Destructor.
	~FreeList() 
	{
		if (m_data) delete m_data;
	}

	// Set the amount of elements that the FreeLists resizes on a resize.
	// This prevents resizing every time an element is added.
	void setResizeIncrements(int increments) 
	{
		m_resizeIncrements = increments;
	}

	// Push element in the next open slot.
	void push(const T& element) 
	{
		// Always check if should resize.
		queryResize(1);


	}

	// Get the capacity.
	int capacity() { return m_capacity; }
	// Get the amount of elements.
	int size()	   { return m_elementCount; }
	// Index operator.
	constexpr T& operator[](int index) { return *(m_data + index); }
	// Const index operator.
	constexpr const T& operator[](int index) const { return *(m_data + index); }

//private:

	// Check if a resize has to occur based on the change.
	// Return true if resize occured.
	bool queryResize(int capacityChange)
	{

	}

	// Allocate the memory.
	void allocate(int newCapacity)
	{
		// Increase allocated data.
		if ( m_data && (newCapacity>m_capacity) )
		{
			// Copy the old data over to a new location.
			T* oldData = m_data;
			m_data = new T[newCapacity];
			memcpy(m_data, oldData, m_capacity);

			// Check if last slot extended to the end.
			if (getSlotNextSlot(m_lastFreeSlot) == -1)
			{
				setSlotData(m_lastFreeSlot, getSlotSize(m_lastFreeSlot) + newCapacity - m_capacity, -1, getSlotPrevSlot(m_lastFreeSlot));
			}
			// Update the slot data.
			else
			{
				setSlotData(m_capacity, newCapacity - m_capacity, m_lastFreeSlot, -1);
			}
			free(oldData);
		}
		// Decrease allocated data.
		else if ( m_data && (newCapacity < m_capacity) ) 
		{
		
		}
		// No data allocated.
		else
		{
			m_data = new T[newCapacity];
			setSlotData(0, newCapacity, -1, -1);
		}
		m_capacity = newCapacity;
	}

	// Get the size of the slot.
	int getSlotSize(int slotIndex)
	{
		return *(getSlotDataPtr(slotIndex));
	}

	// Get the next open slot.
	int getSlotNextSlot(int slotIndex)
	{
		return *(getSlotDataPtr(slotIndex) + 1);
	}

	// Get the prev open slot.
	int getSlotPrevSlot(int slotIndex)
	{
		return *(getSlotDataPtr(slotIndex) + 2);
	}

	// Get the data describing the slot.
	// { size, nextSlot, prevSlot }
	std::tuple<int, int, int> getSlotData(int slotIndex)
	{
		int* data = getSlotDataPtr(slotIndex);
		return { *(data), *(data + 1), *(data + 2) };
	}

	// Set the data describing the next open slot and current slot size.
	void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot)
	{
		int* data = getSlotDataPtr(slotIndex);
		*(data++) = slotSize;
		*(data++) = nextSlot;
		*(data++) = prevSlot;
		updateFreeSlots(slotIndex);
	}

	// Get an int pointer to the slot data.
	int* getSlotDataPtr(int slotIndex)
	{
		return reinterpret_cast<int*>(&(*this)[slotIndex]);
	}

	// Updated the tracked free slots.
	void updateFreeSlots(int slotIndex)
	{
		if (slotIndex < m_firstFreeSlot) m_firstFreeSlot = slotIndex;
		if (slotIndex > m_lastFreeSlot)  m_lastFreeSlot  = slotIndex;
	}

	// --------- //
	//  D A T A  //
	// --------- //

	// The total elements that can be held.
	int m_capacity = 0;
	// Keep track of the amount of elements allocated.
	int m_elementCount = 0;
	// The size (in bytes) of each open slot (each slot can hold one element).
	int m_sizeOfElement = NULL;
	// The index of the slots.
	// -1 = no free slot.
	int m_firstFreeSlot = 0;
	int m_lastFreeSlot  = 0;
	// The amount of slots added/removed on resize.
	int m_resizeIncrements = 10;
	// The ratio that the freelist has to be reduced beyond the resize in increments to be reduced in size.
	// This prevents the situation where resizes happens unneccesarily.
	float m_resizeThreshold = 1.1f;
	// Pointer to the data.
	T* m_data = nullptr;
};