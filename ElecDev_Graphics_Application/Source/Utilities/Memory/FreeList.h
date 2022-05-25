#pragma once

// When data has been freed the first 12 bytes of the slot is used to store
// information regarding the open slots.
// int32 Size			(4 bytes)
// int32 Next Slot		(4 bytes)
// int32 Previous Slot	(4 bytes)
// This means we cannot use glm::vec2.. However, if we make them 21 bits instead
// of 4 bytes we can fit in glm::vec2.  This will make the FreeList max value 2 097 152.
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
	FreeList(int capacity)
		: FreeList()
	{
		allocate(capacity);
	}
	FreeList(int capacity, int capacityIncrements)
		: FreeList(capacity)
	{
		setCapacityIncrements(capacityIncrements);
	}

	// Destructor.
	~FreeList() 
	{
		if (m_data)
		{
			destructors();
			free(m_data);
		}
	}

	// Set the amount of elements that the FreeLists resizes on a resize.
	// This prevents resizing every time an element is added.
	inline void setCapacityIncrements(int increments) 
	{
		m_capacityIncrements = increments;
	}

	// Set the threshold for resizing (more specifically, decreasing).
	// Prevents unnceccesary allocation and copying of data.
	inline void setResizeThreshold(float threshold) 
	{
		m_resizeThreshold = threshold;
	}

	// Push element in the next open slot.
	// Returns the index of the slot start.
	inline int push(const T& element) 
	{
		// Always check if should resize.
		queryResize(1);

		// Find and commit an open slot.
		int slotIndex = findSlotFirstFit(1);
		commitSlot(slotIndex);

		// Copy the data.
		T* ptr = getSlotElementPtr(slotIndex);
		memcpy(ptr, &element, m_sizeOfElement);
		m_elementCount++;

		// Return the index of the element.
		return slotIndex;
	}
	inline int push(T&& element)
	{
		// Always check if should resize.
		queryResize(1);

		// Find and commit an open slot.
		int slotIndex = findSlotFirstFit(1);
		commitSlot(slotIndex);


		return 1;
	}

	// Get the capacity.
	inline constexpr int capacity() { return m_capacity; }
	// Get the amount of elements.
	inline constexpr int size()		{ return m_elementCount; }
	// Index operator.
	inline constexpr T& operator[](int index) { return *(m_data + index); }
	// Const index operator.
	inline constexpr const T& operator[](int index) const { return *(m_data + index); }

//private:

	// Check if a resize has to occur based on the change.
	// Return true if resize occured.
	inline bool queryResize(int elementChange)
	{
		// Ensure actual resize query.
		if (!elementChange) return false;

		// Calculate the required capacity.
		int newElementCount = elementChange + m_elementCount;

		// Increase.
		if ( (elementChange > 0) && newElementCount > m_capacity )
		{
			// The increase value has to be a factor of increase increments.
			int toIncrease = ( ((newElementCount - m_capacity) / m_capacityIncrements) + 1) * m_capacityIncrements;
			allocate(m_capacity + toIncrease);
			return true;
		}

		// Decrease.
		else if ( (newElementCount >= 0) && newElementCount < m_capacity - (m_capacityIncrements * m_resizeThreshold) )
		{
			int toDecrease = (((m_capacity - newElementCount) / m_capacityIncrements) + 1) * m_capacityIncrements;
			allocate(m_capacity - toDecrease);
			return true;
		}

		// No resize occured.
		return false;
	}

	// Allocate the memory.
	inline void allocate(int newCapacity)
	{
		// Increase allocated data.
		if ( m_data && (newCapacity>m_capacity) )
		{
			// Increase capacity.
			T* oldData = m_data;
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			memmove(m_data, oldData, m_capacity * m_sizeOfElement);
			free(oldData);

			// Check if last slot extended to the end.
			if (hasNextSlot(m_lastFreeSlot))
			{
				setSlotData(m_lastFreeSlot, getSlotSize(m_lastFreeSlot) + newCapacity - m_capacity, -1, getPrevSlot(m_lastFreeSlot));
			}
			// Update the slot data.
			else
			{
				// Have more than one slot.
				if (m_lastFreeSlot) setSlotData(m_capacity, newCapacity - m_capacity, m_lastFreeSlot, -1);
				// Have only one slot.
				else			    setSlotData(m_capacity, newCapacity - m_capacity, -1, -1);
			}
		}

		// Decrease allocated data.
		else if ( m_data && (newCapacity<m_capacity) ) 
		{
			// TODO: Decreasing capacity is more complex than increasing.
			// Need to take care of data inside the removed capacity.
			// Remove this functionality for now.
			return;

			// Decrease capacity.
			T* oldData = m_data;
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			memmove(m_data, oldData, newCapacity * m_sizeOfElement);
			free(oldData);

			// Check if last slot extended to the end.
			if (hasNextSlot(m_lastFreeSlot))
			{
				setSlotData(m_lastFreeSlot, getSlotSize(m_lastFreeSlot) - (m_capacity - newCapacity), -1, getPrevSlot(m_lastFreeSlot));
			}
			// Update the slot data.
			else
			{
				// Have more than one slot.
				if (m_lastFreeSlot) setSlotData(m_capacity, newCapacity - m_capacity, m_lastFreeSlot, -1);
				// Have only one slot.
				else			    setSlotData(m_capacity, newCapacity - m_capacity, -1, -1);
			}
		}

		// Empty data.
		else if (!newCapacity)
		{
			if (m_data)
			{
				destructors();
				free(m_data);
			}
			m_data = nullptr;
		}

		// Data has not been allocated yet.
		else 
		{
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			setSlotData(0, newCapacity, -1, -1);
		}

		// Update capacity.
		m_capacity = newCapacity;
	}


	// Get the data describing the slot.
	// { size, nextSlot, prevSlot }
	inline std::tuple<int, int, int> getSlotData(int slotIndex)
	{
		int* data = getSlotDataPtr(slotIndex);
		return { *(data), *(data + 1), *(data + 2) };
	}

	// Set the data describing the next open slot and current slot size.
	inline void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot)
	{
		int* data = getSlotDataPtr(slotIndex);
		*(data++) = slotSize;
		*(data++) = nextSlot;
		*(data++) = prevSlot;
		updateFreeSlots(slotIndex);
	}

	// Updated the tracked free slots.
	inline void updateFreeSlots(int slotIndex)
	{
		if (slotIndex < m_firstFreeSlot) m_firstFreeSlot = slotIndex;
		if (slotIndex > m_lastFreeSlot)  m_lastFreeSlot  = slotIndex;
	}

	// Find the first slot that fits the data.
	inline int findSlotFirstFit(int slotSize) 
	{
		int slotIndex = m_firstFreeSlot;
		int currentSlotSize = getSlotSize(slotIndex);
		while (currentSlotSize < slotSize)
		{
			slotIndex = getNextSlot(slotIndex);
			slotSize = getSlotSize(slotIndex);
		}
		return slotIndex;
	}

	// Commit a slot to be used.
	inline void commitSlot(int slotIndex) 
	{
		// If it has no next slot, but is not the last slot in the list
		// we need to update the previous slot to point to the slot after
		// the current one.
		if (!hasNextSlot(slotIndex))
		{
			if (slotIndex < m_capacity - 1)
			{
				if(hasPrevSlot(slotIndex))
			}
		}
		else 
		{
			
		}

		// Prev slot.
		if (!hasPrevSlot(slotIndex))
		{

		}
		else 
		{
		
		}
	}

	// Free a used slot.
	inline void freeSlot(int slotIndex) 
	{
		updateFreeSlots(slotIndex);
	}

	// Call the destructors of all of the elements.
	inline void destructors() 
	{
		
	}

	// Slot utilities.
	inline int getSlotSize(int slotIndex)				 { return *(getSlotDataPtr(slotIndex)); }				// Get the size of the slot.
	inline int getNextSlot(int slotIndex)				 { return *(getSlotDataPtr(slotIndex) + 1); }			// Get the next open slot.
	inline int getPrevSlot(int slotIndex)				 { return *(getSlotDataPtr(slotIndex) + 2); }			// Get the prev open slot.
	inline void setSlotSize(int slotIndex, int size)	 { *(getSlotDataPtr(slotIndex)) = size; }				// Set the size of the slot.
	inline void setNextSlot(int slotIndex, int nextSlot) { *(getSlotDataPtr(slotIndex) + 1) = nextSlot; }		// Set the next open slot.
	inline void setPrevSlot(int slotIndex, int prevSlot) { *(getSlotDataPtr(slotIndex) + 2) = prevSlot; }		// Set the prev open slot.
	inline bool hasNextSlot(int slotIndex)				 { return getNextSlot(slotIndex) != -1; }				// Check if the given slot has a next slot.
	inline bool hasPrevSlot(int slotIndex)				 { return getPrevSlot(slotIndex) != -1; }				// Check if the given slot has a previous slot.
	inline int* getSlotDataPtr(int slotIndex)			 { return reinterpret_cast<int*>(m_data + slotIndex); }	// Get an int pointer to the slot data.
	inline T* getSlotElementPtr(int slotIndex)			 { return &(*this)[slotIndex]; }						// Get a pointer to the slot as an element.

	// --------- //
	//  D A T A  //
	// --------- //

	T* m_data = nullptr;			// Pointer to the data on the heap.
	int m_capacity = 0;				// The total elements that can be held.
	int m_elementCount = 0;			// Keep track of the amount of elements allocated.
	int m_sizeOfElement = NULL;		// The size (in bytes) of each open slot (each slot can hold one element).
	int m_firstFreeSlot = 0;		// The indices of the slots,
	int m_lastFreeSlot  = 0;		// -1 = no free slot.
	int m_capacityIncrements = 10;	// The amount of slots added/removed on resize.
	float m_resizeThreshold = 1.1f; // The ratio that the freelist has to be reduced beyond the resize in increments to be reduced in size.
									// This prevents the situation where resizes happens unneccesarily.
};