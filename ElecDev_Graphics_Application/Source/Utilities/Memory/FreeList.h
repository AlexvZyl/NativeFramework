#pragma once

#include <tuple>
#include <assert.h>

// When data has been freed the first 12 bytes of the slot is used to store
// information regarding the open slots.
// int32 Size			(4 bytes)
// int32 Next Slot		(4 bytes)
// int32 Previous Slot	(4 bytes)
// This means we cannot use glm::vec2...
#define MINIMUM_FREELIST_ELEMENT_SIZE 12 // Bytes.
#define FREELIST_MAX_CAPACITY 2^32

template<typename T>
class FreeList 
{

public:

	// The modes of iteration.
	enum class IteratorMode { ELEMENTS, MEMORY };

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
		: FreeList()
	{
		setCapacityIncrements(capacityIncrements);
		allocate(capacity);
	}

	// Destructor.
	~FreeList() 
	{
		if (m_data)
		{
			callAllDestructors();
			free(m_data);
		}
	}

	// Clear the list of all its elements.
	inline void clear()
	{
		callAllDestructors();
		m_firstFreeSlot = 0;
		m_lastFreeSlot = 0;
		m_elementCount = 0;
		setSlotData(0, m_capacity, -1, -1);
	}

	// Push element in the next open slot.
	// Returns the index of the slot start.
	inline int push(const T& element) 
	{
		// Find and commit an open slot.
		int slotIndex = findSlotFirstFit();
		commitSlot(slotIndex);

		// Do in place copy construction.
		new (getSlotElementPtr(slotIndex)) T(element);

		// Return the index of the element.
		return slotIndex;
	}

	// Push an array of elements into the list.
	// Returns the index of the start.
	// The memory is guaranteed to be contiguous.
	inline int push(T* elementPtr, int size) 
	{
		// Find and commit slot range.
		int slotIndex = findSlotFirstFit(size);
		commitSlot(slotIndex, size);

		// Do in place copy construction.
		for (int i = 0; i < size; i++)
			new (getSlotElementPtr(slotIndex+i)) T(*(elementPtr+i));

		// Return the index of the first element.
		return slotIndex;
	}

	// Push element in the next open slot.
	// Returns the index of the slot start.
	inline int push(T&& element)
	{
		// Find and commit an open slot.
		int slotIndex = findSlotFirstFit();
		commitSlot(slotIndex);

		// Do in place copy construction.
		new (getSlotElementPtr(slotIndex)) T(element);

		// Return the index of the element.
		return slotIndex;
	}

	// Push element in the next open slot.
	// Returns the index of the slot start.
	template<class ... Args>
	inline int emplace(const Args& ... args)
	{
		// Find and commit an open slot.
		int slotIndex = findSlotFirstFit();
		commitSlot(slotIndex);

		// Do in place construction.
		new (getSlotElementPtr(slotIndex)) T(args...);

		// Return the index of the element.
		return slotIndex;
	}

	// Pop the element at the given index.
	inline void erase(int index) 
	{
		assert(index < m_capacity, "Trying to pop outside of capacity.");
		slotDestructor(index);
		freeSlot(index);
	}

	// Pop the range of elements.
	inline void erase(int startIndex, int size)
	{
		assert(startIndex + size < m_capacity, "Trying to pop outside of capacity.");
		for (int i = startIndex; i < startIndex + size; i++)
			slotDestructor(i);
		freeSlot(startIndex, size);
	}
	
	// Utilities.
	inline constexpr int capacity()						{ return m_capacity; }					// Get the capacity in amount of elements.
	inline constexpr int size()							{ return m_elementCount; }				// Get the amount of elements comitted.
	inline void setCapacityIncrements(int increments)   { m_capacityIncrements = increments; }	// Set the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline int getCapacityIncrements()					{ return m_capacityIncrements; }		// Get the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline void setResizeThreshold(float threshold)		{ m_resizeThreshold = threshold; }		// Set the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline float getResizeThreshold()					{ return m_resizeThreshold; }			// Get the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline void setIteratorMode(IteratorMode mode)		{ m_iteratorMode = mode; }				// Set the mode of iteration.
	inline IteratorMode getIteratorMode()				{ return m_iteratorMode; }				// Get the mode of iteration.

	// Index operator.
	inline constexpr T& operator[](int index)			  
	{
		#ifdef _DEBUG
			assert(index >= 0 && index < capacity(), "Trying to access data outside of the FreeList.");
		#endif
		return *(m_data + index); 
	} 
	// Const index operator.
	inline constexpr const T& operator[](int index) const 
	{ 
		#ifdef _DEBUG
			assert(index >= 0 && index < capacity(), "Trying to access data outside of the FreeList.");
		#endif
		return *(m_data + index); 
	} 

private:

	friend class Iterator;

	// Check if a resize has to occur based on the change.
	// Return true if resize occured.
	inline bool queryResize(int elementChange)
	{
		// Ensure actual resize query.
		if (!elementChange) return false;

		// Calculate the expected element count with query.
		int newElementCount = elementChange + m_elementCount;

		// Increase.
		if ( (elementChange > 0) && newElementCount > m_capacity )
		{
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
		// Check for overflow.
		assert(newCapacity < FREELIST_MAX_CAPACITY, "Exceeded FreeList maximum capacity.");

		// Increase allocated data.
		if (m_data && (newCapacity > m_capacity))
		{
			// Increase capacity.
			T* oldData = m_data;
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			memmove(m_data, oldData, m_capacity * m_sizeOfElement);
			free(oldData);

			// Did not have a last slot or last free slot is not at the 
			// end of memory, so use the new memory for it.
			if (!lastFreeSlotValid() || m_lastFreeSlot != m_capacity - 1)
			{
				// Set new slot data.
				if (!firstFreeSlotValid()) setSlotData(m_capacity, newCapacity - m_capacity, -1, -1);
				else					   setSlotData(m_capacity, newCapacity - m_capacity, -1, m_firstFreeSlot);

				// Update.
				updateFreeSlots(m_capacity);
			}

			// Has a last free slot at the end of memory, so simply increase size.
			else setSlotData(m_lastFreeSlot, getSlotSize(m_lastFreeSlot) + newCapacity - m_capacity, -1, getPrevSlot(m_lastFreeSlot));
		}

		// Decrease allocated data.
		else if (m_data && (newCapacity < m_capacity))
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

		// Empty existing data.
		else if (!newCapacity && m_data)
		{
			callAllDestructors();
			free(m_data);
			m_firstFreeSlot = -1;
			m_lastFreeSlot = -1;
			m_elementCount = 0;
			m_data = nullptr;
		}

		// Increase with no existing data.
		else if (!m_data && newCapacity)
		{
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			setSlotData(0, newCapacity, -1, -1);
			updateFreeSlots(0);
		}

		// No allocation required.
		else if (!m_data && !newCapacity) return;

		// Error.
		else assert(false, "Error with memory allocation.");

		// Update capacity.
		m_capacity = newCapacity;
	}

	// Find the first slot that fits the data.
	inline int findSlotFirstFit(int slotSize = 1) 
	{
		// Find an open slot that is large enough, starting from the first free one.
		// If there is no valid first slot, skip the initial search and resize.
		int slotIndex = m_firstFreeSlot;
		if (slotIsValid(slotIndex))
		{
			int currentSlotSize = getSlotSize(slotIndex);
			while (currentSlotSize < slotSize)
			{
				slotIndex = getNextSlot(slotIndex);
				slotSize = getSlotSize(slotIndex);
			}
		}

		// Found a valid slot.
		if (slotIsValid(slotIndex)) return slotIndex;

		// Did not find a valid slot.  Resize and search again.
		if(queryResize(slotSize)) return findSlotFirstFit(slotSize);

		// Failed.
		assert(false, "Slot find and resize both failed.");
	}

	// Commit a slot to be used.
	inline void commitSlot(int slotIndex, int size = 1) 
	{
		// Comitting a slot means the elements are added.
		m_elementCount+=size;

		// Get the slot data.
		auto [slotSize, nextSlot, prevSlot] = getSlotData(slotIndex);
		bool curHasPrevSlot = hasPrevSlot(slotIndex);
		bool curHasNextSlot = hasNextSlot(slotIndex);

		// Ensure slot is large enough.
		assert(slotSize >= size, "Slot cannot hold the requested data.");

		// Slot is larger than the requested size.
		if (slotSize > size)
		{
			// The next open slot is now the new slot.
			setSlotData(slotIndex + size, slotSize - size, nextSlot, prevSlot);

			// Update prev slot data.
			if (curHasPrevSlot) setNextSlot(prevSlot, slotIndex + size);
			else				m_firstFreeSlot = slotIndex + size;

			// Update next slot data.
			if (curHasNextSlot) setPrevSlot(nextSlot, slotIndex + size);
			else				m_lastFreeSlot = slotIndex + size;
		}

		// Size consumes entire slot.
		else
		{
			// Update prev slot data.
			if (curHasPrevSlot) setNextSlot(prevSlot, nextSlot);
			else				m_firstFreeSlot = nextSlot;

			// Update next slot data.
			if (curHasNextSlot)	setPrevSlot(nextSlot, prevSlot);
			else				m_lastFreeSlot = prevSlot;
		}
	}

	// Free a used slot.
	inline void freeSlot(int slotIndex, int size = 1) 
	{
		// Freeing a slot means an element has been removed.
		m_elementCount-=size;
		assert(m_elementCount >= 0, "Tried to free too many elements.");

	#ifdef _DEBUG
		// Clear the memorty.
		setSlotsMemory(slotIndex, size);
	#endif 

		// Find the slots next to the inserted one.
		auto [prevSlot, nextSlot] = findAdjacentSlots(slotIndex); 

		// Set slot data.
		setSlotData(slotIndex, size, nextSlot, prevSlot);
		updateFreeSlots(slotIndex);

		// Prev slot.
		if (slotIsValid(prevSlot))
		{
			// Update.
			setNextSlot(prevSlot, slotIndex);
			// Merge.
			if (prevSlot + getSlotSize(prevSlot) == slotIndex) mergeSlots(prevSlot, slotIndex);
		}
		// Next slot.
		if (slotIsValid(nextSlot))
		{
			// Update.
			setPrevSlot(nextSlot, slotIndex);
			// Merge.  Here we take nextSlots' prev slot (instead of slotIndex) because it 
			// is possible that merge was already run on slotIndex.
			if (slotIndex + size == nextSlot) mergeSlots(getPrevSlot(nextSlot), nextSlot);
		}

		// Check if it should shrink.
		queryResize(-1);
	}

	// Call the destructors of all of the elements.
	inline void callAllDestructors() 
	{
		IteratorMode tmp = m_iteratorMode;
		setIteratorMode(IteratorMode::ELEMENTS);
		for (auto& element : *this) element.~T();
		setIteratorMode(tmp);
	}

	// Set the data describing the next open slot and current slot size.
	inline void setSlotData(int slotIndex, int slotSize, int nextSlot, int prevSlot)							  
	{
		int* data = getSlotDataPtr(slotIndex);
		*(data++) = slotSize;
		*(data++) = nextSlot;
		*(data  ) = prevSlot;
	}

	// Get the data describing the slot.
	// { size, nextSlot, prevSlot }
	inline std::tuple<int, int, int> getSlotData(int slotIndex)													
	{																											
		int* data = getSlotDataPtr(slotIndex);
		return { *(data), *(data + 1), *(data + 2) };
	}

	// Updated the tracked free slots.
	inline void updateFreeSlots(int slotIndex)																	
	{
		if (slotIndex < m_firstFreeSlot || !firstFreeSlotValid()) m_firstFreeSlot = slotIndex;
		if (slotIndex > m_lastFreeSlot  || !lastFreeSlotValid())  m_lastFreeSlot  = slotIndex;
	}

	// Find the slots adjacent to the passed slot.  This is used when inserting a slot and having to update the data.			
	inline std::tuple<int, int> findAdjacentSlots(int slotIndex)												
	{
		// Check for available slots.
		bool hasValidFirst = firstFreeSlotValid();
		bool hasValidLast = lastFreeSlotValid();

		// Default to no slot.
		int leftSlot = -1;
		int rightSlot = -1;

		// No valid slots, return defaults.
		if (!hasValidFirst && !hasValidLast) { }

		// If both slots are valid, use the one closest to the index.
		else if (hasValidFirst && hasValidLast)
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

		// Only left slot is valid.
		else if (hasValidFirst)
		{
			int referenceSlot = searchForward(m_firstFreeSlot, slotIndex);
			leftSlot = referenceSlot;
			rightSlot = getNextSlot(referenceSlot);
		}

		// Only right slot is valid.
		else
		{
			int referenceSlot = searchReverse(m_lastFreeSlot, slotIndex);
			rightSlot = referenceSlot;
			leftSlot = getPrevSlot(referenceSlot);
		}

		// Return the slots.
		return { leftSlot, rightSlot };
	}

	// Search for the slot that sits right before the reference slot, starting at the startSlot.
	inline int searchForward(int startSlot, int referenceSlot) 
	{
		int currentSlot = startSlot;
		int currentNextSlot = getNextSlot(currentSlot);
		while (currentNextSlot < referenceSlot && slotIsValid(currentNextSlot))
		{
			currentSlot = currentNextSlot;
			currentNextSlot = getNextSlot(currentSlot);
		}
		return currentSlot;
	}

	// Search for the slot that sits right after the reference slot, starting at the startSlot.
	inline int searchReverse(int startSlot, int referenceSlot)
	{
		int currentSlot = startSlot;
		int currentPrevSlot = getPrevSlot(currentSlot);
		while (currentPrevSlot > referenceSlot && slotIsValid(currentPrevSlot))
		{
			currentSlot = currentPrevSlot;
			currentPrevSlot = getNextSlot(currentSlot);
		}
		return currentSlot;
	}

	// Merge the two slots.  Called when two slots are next to each other in memory.
	inline void mergeSlots(int firstSlot, int secondSlot) 
	{
		increaseSlotSize(firstSlot, getSlotSize(secondSlot));
		setNextSlot(firstSlot, getNextSlot(secondSlot));
		if (hasNextSlot(secondSlot)) setPrevSlot(getNextSlot(secondSlot), firstSlot);
		if (m_lastFreeSlot == secondSlot) m_lastFreeSlot = firstSlot;
	}

	// Slot utilities.
	inline int getSlotSize(int slotIndex)								{ return *(getSlotDataPtr(slotIndex)); }								// Get the size of the slot.
	inline int getNextSlot(int slotIndex)								{ return *(getSlotDataPtr(slotIndex) + 1); }							// Get the next open slot.
	inline int getPrevSlot(int slotIndex)								{ return *(getSlotDataPtr(slotIndex) + 2); }							// Get the prev open slot.
	inline void setSlotSize(int slotIndex, int size)					{ *(getSlotDataPtr(slotIndex)) = size; }								// Set the size of the slot.
	inline void setNextSlot(int slotIndex, int nextSlot)				{ *(getSlotDataPtr(slotIndex) + 1) = nextSlot; }						// Set the next open slot.
	inline void setPrevSlot(int slotIndex, int prevSlot)				{ *(getSlotDataPtr(slotIndex) + 2) = prevSlot; }						// Set the prev open slot.
	inline bool hasNextSlot(int slotIndex)								{ return getNextSlot(slotIndex) != -1; }								// Check if the given slot has a next slot.
	inline bool hasPrevSlot(int slotIndex)								{ return getPrevSlot(slotIndex) != -1; }								// Check if the given slot has a previous slot.
	inline int* getSlotDataPtr(int slotIndex)							{ return reinterpret_cast<int*>(m_data + slotIndex); }					// Get an int pointer to the slot data.
	inline T* getSlotElementPtr(int slotIndex)							{ return m_data+slotIndex; }											// Get a pointer to the slot as an element.
	inline bool firstFreeSlotValid()									{ return m_firstFreeSlot != -1; }										// Checks if the first free slot is valid (exists).
	inline bool lastFreeSlotValid()										{ return m_lastFreeSlot != -1; }										// Checks if the last free slot is valid (exists).
	inline bool slotIsValid(int slot)									{ return slot != -1; }													// Checks if the slot is valid.
	inline void slotDestructor(int slotIndex)							{ getSlotElementPtr(slotIndex)->~T(); }									// Call the destructor of the element in the slot.
	inline void copyToSlot(T* source, int slotIndex)					{ memcpy(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Copy the element data into the slot.
	inline void copyToSlot(const T* source, int slotIndex)				{ memcpy(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Copy the element data into the slot.
	inline void moveToSlot(T* source, int slotIndex)					{ memmove(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Move the element data into the slot.
	inline void moveToSlot(const T* source, int slotIndex)				{ memmove(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Move the element data into the slot.
	inline void increaseSlotSize(int slotIndex, int size)				{ setSlotSize(slotIndex, getSlotSize(slotIndex) + size); }				// Increase the slot size.
	inline void setSlotMemory(int slotIndex, int val = 0xCC)			{ setSlotsMemory(slotIndex, 1, val); }									// Set the value of the slot memory.
	inline void setSlotsMemory(int slotIndex, int size, int val = 0xCC)	{ memset(getSlotElementPtr(slotIndex), val, m_sizeOfElement * size); }	// Set the value of the slots memory.

	// Data.
	T* m_data = nullptr;			// Pointer to the data on the heap.
	int m_capacity = 0;				// The total elements that can be held.
	int m_elementCount = 0;			// Keep track of the amount of elements allocated.
	int m_sizeOfElement = NULL;		// The size (in bytes) of each open slot (each slot can hold one element).
	int m_firstFreeSlot = -1;		// The indices of the slots,
	int m_lastFreeSlot  = -1;		// -1 = no free slot.
	int m_capacityIncrements = 10;	// The amount of slots added/removed on resize.
	float m_resizeThreshold = 1.1f; // The ratio that the freelist has to be reduced beyond the resize in increments to be reduced in size.
									// This prevents the situation where resizes happens unneccesarily.
	IteratorMode m_iteratorMode = IteratorMode::ELEMENTS; // The mode of iteration.

public:

	// FreeLists need a custom iterator, since we want to be able to avoid unused 
	// slots and set the mode of iteration.
	class Iterator
	{
	public:

		// Contructor.
		Iterator(FreeList<T>* fl, int index, int nextFreeSlot, IteratorMode mode)
		{
			m_freeList = fl;
			m_nextFreeSlot = nextFreeSlot;
			m_index = index;
			m_iteratorMode = mode;
		}

		// Operators.
		inline void operator++()
		{
			switch (m_iteratorMode)
			{

			case IteratorMode::ELEMENTS:
				// Next slot contains data.
				if (m_nextFreeSlot == -1 || m_index + 1 < m_nextFreeSlot)
				{
					m_index++;
				}
				// Next slot does not contain data.
				else
				{
					// 'Jump' over slot.
					m_index += m_freeList->getSlotSize(m_nextFreeSlot) + 1;
					// Set the next slot to miss.
					m_nextFreeSlot = m_freeList->getNextSlot(m_nextFreeSlot);
				}
				break;

			case IteratorMode::MEMORY:
				break;

			default:
				assert(false, "Invalid iterator mode.");
				break;
			}
		}
		inline bool operator!=(const Iterator& other) { return m_index != other.m_index; }
		inline const T& operator*() const { return (*m_freeList)[m_index]; }
		inline T& operator*() { return (*m_freeList)[m_index]; }

	private:

		// Data.
		int m_nextFreeSlot = NULL;			// Keep track of the free slots as we iterate.
		FreeList<T>* m_freeList = nullptr;	// FreeList iterating over.
		int m_index = NULL;					// Current point in iteration.
		IteratorMode m_iteratorMode = IteratorMode::ELEMENTS;
	};

	// Iterator functions.
	inline Iterator begin() 
	{ 
		// First slot in memory.
		if (m_firstFreeSlot == 0) return Iterator(this, getSlotSize(m_firstFreeSlot), getNextSlot(m_firstFreeSlot), m_iteratorMode);
		// Construct iterator normally.
		else					  return Iterator(this, 0, m_firstFreeSlot, m_iteratorMode);
	}
	inline Iterator end()   
	{ 
		return Iterator(this, capacity(), -1, m_iteratorMode); 
	}
};