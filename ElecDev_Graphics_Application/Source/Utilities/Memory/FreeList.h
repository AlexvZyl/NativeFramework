#pragma once

#include <tuple>
#include <assert.h>

// When there is a slot in memory that is not being used by the FreeList, 
// it is used as a member in the FreeList 'LinkedList'. 
// Ordering the memory leads to less fragmentation.
// The memory is laid out as seen below:
//
//	-------------------------------------------------------------------------
//	|	    	4		    |			4			|			4			|
//	|	     [Size]		    |	   [Prev slot]		|	   [Next slot]		|
//	-------------------------------------------------------------------------
//  
// Size: The size of the current slot.    
// Next slot: The index if the following slot.
// Prev slot: The index if the previous slot.

#define FREELIST_MINIMUM_ELEMENT_SIZE 12 // Bytes.
#define FREELIST_MAX_CAPACITY 2147483648 // 2^31 for int.

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
        // The FreeList element must be larger than or equal to 12 bytes.
		static_assert(sizeof(T) >= FREELIST_MINIMUM_ELEMENT_SIZE); 
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

	// Copy constructor.
	FreeList(const FreeList& other)
	{
		// Copy metadata.
		setCapacityIncrements(other.getCapacityIncrements());
		setResizeThreshold(other.getResizeThreshold());
		setIteratorMode(other.getIteratorMode());
		allocate(other.capacity());
		m_elementCount = other.m_elementCount;
		m_firstFreeSlot = other.m_firstFreeSlot;
		m_lastFreeSlot = other.m_lastFreeSlot;

		// Copy the elements with their copy constructors.
		auto it = other.begin();
		it.m_iteratorMode = IteratorMode::ELEMENTS;
		for(it; it != other.end(); ++it)
		{
			new (getSlotElementPtr(it.m_index)) T(*it);
		}

		// Now copy all the slots' data.
		int currentSlot = other.m_firstFreeSlot;
		while(slotIsValid(currentSlot))
		{
			copySlotData(other.getSlotDataPtr(currentSlot), currentSlot);			
			currentSlot = other.getNextSlot(currentSlot);
		}
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
    // Restores everything to one large slot.
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
	inline int push(const T* elementPtr, int size) 
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

	// Pop the range of elements.
	inline void erase(int startIndex, int size = 1)
	{
		assert(startIndex + size <= m_capacity); // Trying to pop outside of capacity.
		freeSlot(startIndex, size);
	}
	
	// Utilities.
	inline constexpr int capacity() const               { return m_capacity; }						 // Get the capacity in amount of elements.
	inline constexpr int allocated() const              { return capacity() * m_sizeOfElement; }	 // Get the amount of memory used in bytes.
	inline constexpr int count() const                  { return m_elementCount; }					 // Get the amount of elements comitted.
	inline constexpr int size() const                   { return count() * m_sizeOfElement; }		 // Get the amount of data used (in bytes).
	inline void setCapacityIncrements(int increments)   { m_capacityIncrements = increments; }		 // Set the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline int getCapacityIncrements() const            { return m_capacityIncrements; }			 // Get the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline void setResizeThreshold(float threshold)     { m_resizeThreshold = threshold; }			 // Set the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline float getResizeThreshold() const             { return m_resizeThreshold; }				 // Get the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline void setIteratorMode(IteratorMode mode)      { m_iteratorMode = mode; }					 // Set the mode of iteration.
	inline IteratorMode getIteratorMode() const         { return m_iteratorMode; }					 // Get the mode of iteration.
	inline void iterateMemory() 						{ m_iteratorMode = IteratorMode::MEMORY; }	 // Set the FreeList to iterate over memory regions.
	inline void iterateElements() 						{ m_iteratorMode = IteratorMode::ELEMENTS; } // Set the FreeList to iterate over elements in memory.

	// Index operator.
	inline constexpr T& operator[](int index)			  
	{
		#ifdef _DEBUG
        assert(index >= 0 && index < capacity()); // Trying to access data outside of the FreeList.
		#endif
		return *(m_data + index); 
	} 
	// Const index operator.
	inline constexpr const T& operator[](int index) const 
	{ 
		#ifdef _DEBUG
        assert(index >= 0 && index < capacity()); // Trying to access data outside of the FreeList.
		#endif
		return *(m_data + index); 
	} 

private:

	friend class Iterator;

	// Tries to resize based on the requested capacity change.
	// Takes into account the capacity increments value.
	inline bool queryResize(int capacityChange)
	{
		// Ensure actual resize query.
		if (!capacityChange) return false;

		// Calculate the requested new capacity.
		int newCapacity = capacityChange + m_capacity;

		// Increase.
		if ( (capacityChange > 0) && newCapacity > m_capacity )
		{
			int toIncrease = ( ((newCapacity - m_capacity) / m_capacityIncrements) + 1) * m_capacityIncrements;
			allocate(m_capacity + toIncrease);
			return true;
		}

		// Decrease.
		else if ( (newCapacity >= 0) && newCapacity < m_capacity - (m_capacityIncrements * m_resizeThreshold) )
		{
			int toDecrease = (((m_capacity - newCapacity) / m_capacityIncrements) + 1) * m_capacityIncrements;
			allocate(m_capacity - toDecrease);
			return true;
		}

		// No resize occured.
		return false;
	}

	// Resizes the freelist so that it can fit an element of the specified size.
	// This function assumes that findSlotFirstFit() was run and a slot large enough was
	// not found.  It will therefore try to use the last slot (if it sits at the end) first
	// before resizing the entire capacity.
	inline bool resizeToFitElement(int size = 1) 
	{
		// Try to add to the last slot.
		if (lastFreeSlotValid() && isLastSlotAtEnd())	
			return queryResize(size - getSlotSize(m_lastFreeSlot));
		// Allocate new memory.
		else return queryResize(size);
	}

	// Allocate the memory.
	inline void allocate(int newCapacity)
	{
		// Check for overflow.
		assert(newCapacity < FREELIST_MAX_CAPACITY); // Exceeded FreeList maximum capacity.

		// Increase allocated data.
		if (m_data && (newCapacity > m_capacity))
		{
			// Increase capacity.
			T* oldData = m_data;
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			assert(m_data); // Could not allocate memory.
			memmove(m_data, oldData, m_capacity * m_sizeOfElement);
			free(oldData);

            // Did not have a last slot.  
			// This also means the new slot will be the only slot.
            if(!lastFreeSlotValid())
            {
                setSlotData(m_capacity, newCapacity - m_capacity, -1, -1);
                updateFreeSlots(m_capacity);
            }

			// Has a last free slot.
			else
			{
				int prevLastSlot = m_lastFreeSlot;
				// Create new slot.
				setSlotData(m_capacity, newCapacity - m_capacity, m_lastFreeSlot, -1);
				updateFreeSlots(m_capacity);
				// Try to merge with the last slot.
				attemptMerge(prevLastSlot, m_capacity);
			}
		}

		// Decrease allocated data.
		else if (m_data && (newCapacity < m_capacity))
		{
			// TODO: Decreasing capacity is more complex than increasing.
			// Need to take care of data inside the removed capacity.
			// Remove this functionality for now.
			newCapacity = m_capacity; // Ignore resize for now.
			return;

			// Decrease capacity.
			T* oldData = m_data;
			m_data = (T*)malloc(newCapacity * m_sizeOfElement);
			assert(m_data); // Could not allocate memory.
			memmove(m_data, oldData, newCapacity * m_sizeOfElement);
			free(oldData);
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
			assert(m_data); // Could not allocate memory.
			setSlotData(0, newCapacity, -1, -1);
			updateFreeSlots(0);
		}

		// No allocation required.
		else if (!m_data && !newCapacity) return;

		// Error with memory allocation.    
		else assert(false); 

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
            // Look for a large enough slot.
			while (currentSlotSize < slotSize)
			{
				// Get next slot and check if is valid.
				slotIndex = getNextSlot(slotIndex);
				if (!slotIsValid(slotIndex)) break;
				currentSlotSize = getSlotSize(slotIndex);
			}
            // Found a valid slot.
		    if (slotIsValid(slotIndex)) return slotIndex;
		}

		// Did not find a valid slot.  Resize and return the last slot.
		if(resizeToFitElement(slotSize)) return m_lastFreeSlot;

		// Slot find and resize both failed.
		assert(false); 
        return 0;
	}

	// Commit a slot to be used.
	inline void commitSlot(int slotIndex, int size = 1) 
	{
		// Comitting a slot means the elements are added.
		m_elementCount+=size;

		// Get the slot data.
		auto [slotSize, prevSlot, nextSlot] = getSlotData(slotIndex);
		bool curHasPrevSlot = hasPrevSlot(slotIndex);
		bool curHasNextSlot = hasNextSlot(slotIndex);

        // Assert if the slot cannot hold the requested data.
		assert(slotSize >= size);  

		// Slot is larger than the requested size.
		if (slotSize > size)
		{
			// Change the used slot data.
			setSlotData(slotIndex + size, slotSize - size, prevSlot, nextSlot);

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
			// Try to connect.
			if(attemptConnection(prevSlot, nextSlot)) return;

			// Update edge slots.
			if(!curHasPrevSlot) m_firstFreeSlot = nextSlot;
			if(!curHasNextSlot) m_lastFreeSlot = prevSlot;
		}
	}

	// Free a used slot.
	inline void freeSlot(int slotIndex, int size = 1) 
	{
		// Find the slots next to the inserted one.
		auto [prevSlot, nextSlot] = findAdjacentSlots(slotIndex); 

	#ifdef _DEBUG
		// Check if the slot has already been freed.
		if (slotIsValid(prevSlot)) assert(!isSlotContained(prevSlot, slotIndex));		// Slot has already been freed.
		if (slotIsValid(nextSlot)) assert(!isSlotContained(slotIndex, size, nextSlot)); // Slot has already been freed.
	#endif

		// Destroy slot elements.
		for(int i=slotIndex; i<slotIndex+size; i++) slotDestructor(i);
		// Freeing a slot means an element has been removed.
		m_elementCount-=size;
        // Assert if tried to free too many elements.
		assert(m_elementCount >= 0); 

	#ifdef _DEBUG
		// Clear the memory.
		setSlotsMemory(slotIndex, size);
	#endif 

		// Set slot data.
		setSlotData(slotIndex, size, prevSlot, nextSlot);
		updateFreeSlots(slotIndex);

		// NOTE: The following 4 lines can be optimised by adding if statements.
		// It is kept like this for readability and since freelists already excel 
		// at erasing.  
		// (A connection is uneccesary if it is going to be merged).

		// Try to connect slots.
		attemptConnection(prevSlot, slotIndex);
		attemptConnection(slotIndex, nextSlot);

		// Try to merge slots.
		attemptMerge(prevSlot, slotIndex);
		attemptMergeWithPrevSlot(nextSlot);
			
		// Check if it should shrink.
		// For now this will do nothing.
		queryResize(-size);
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
	inline void setSlotData(int slotIndex, int slotSize, int prevSlot, int nextSlot)							  
	{
		int* data = getSlotDataPtr(slotIndex);
		*(data++) = slotSize;
		*(data++) = prevSlot;
		*(data  ) = nextSlot;
	}

	// Get the data describing the slot.
	// { Size, Prev Slot, Next Slot }
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

		// If not slots are valid these defaults will be returned.
		int leftSlot = -1;
		int rightSlot = -1;

		// If both slots are valid, use the one closest to the index.
		if (hasValidFirst && hasValidLast)
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
		else if (hasValidLast)
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

    // Checks if a merge is possible based on their adjacency and validity.
    // Merges if it is valid.
	inline bool attemptMerge(int firstSlot, int secondSlot) 
	{
		// Ensure slots are valid.
		if(!slotIsValid(firstSlot) || !slotIsValid(secondSlot)) return false;

		// Ensure slots are adjacent.
		if(!slotsAreAdjacent(firstSlot, secondSlot)) return false;

		// Merge is possible.
		mergeSlots(firstSlot, secondSlot);
		return true;
	}

	// Try to merge the current slot with its prev slot.
	inline bool attemptMergeWithPrevSlot(int slotIndex)
	{
		if(!slotIsValid(slotIndex)) return false;

		return attemptMerge(getPrevSlot(slotIndex), slotIndex);
	}

	// Try to merge the current slot with its next slot.
	inline bool attemptMergeWithNextSlot(int slotIndex)
	{
		if(!slotIsValid(slotIndex)) return false;

		return attemptMerge(slotIndex, getNextSlot(slotIndex));
	}

	// Try to connect two slots.  Checks if they are valid.
	inline bool attemptConnection(int firstSlot, int secondSlot)
	{
		if(!slotIsValid(firstSlot) || !slotIsValid(secondSlot)) return false;
		connectSlots(firstSlot, secondSlot);
		return true;
	}

	// Merge the two slots.  Called when two slots are next to each other in memory.
    // Does not check if the merge is valid.
	inline void mergeSlots(int firstSlot, int secondSlot) 
	{
		increaseSlotSize(firstSlot, getSlotSize(secondSlot));
		setNextSlot(firstSlot, getNextSlot(secondSlot));
		if (hasNextSlot(secondSlot)) setPrevSlot(getNextSlot(secondSlot), firstSlot);
		if (m_lastFreeSlot == secondSlot) m_lastFreeSlot = firstSlot;
	}

    // Memoory.
	inline int* getSlotDataPtr(int slotIndex) 							{ return reinterpret_cast<int*>(m_data + slotIndex); }					// Get an int pointer to the slot data.
	inline const int* getSlotDataPtr(int slotIndex) const 				{ return reinterpret_cast<int*>(m_data + slotIndex); }					// Get an const int pointer to the slot data.
	inline T* getSlotElementPtr(int slotIndex) 							{ return m_data + slotIndex; }											// Get a pointer to the slot as an element.
	inline const T* getSlotElementPtr(int slotIndex) const				{ return m_data + slotIndex; }											// Get a const pointer to the slot as an element.
	inline void setSlotMemory(int slotIndex, int val = 0xCC)			{ setSlotsMemory(slotIndex, 1, val); }									// Set the value of the slot memory.
	inline void setSlotsMemory(int slotIndex, int size, int val = 0xCC)	{ memset(getSlotElementPtr(slotIndex), val, m_sizeOfElement * size); }	// Set the value of the slots memory.
	inline void copySlotData(int* source, int slotDest)   				{ memcpy(getSlotDataPtr(slotDest), source, 12); }						// Copy the data from another slot to this slot.
	inline void copySlotData(const int* source, int slotDest) 			{ memcpy(getSlotDataPtr(slotDest), source, 12); }						// Copy the data from another slot to this slot.

	// Slot utilities.
	inline void slotDestructor(int slotIndex)							{ getSlotElementPtr(slotIndex)->~T(); }									// Call the destructor of the element in the slot.
	inline int getSlotSize(int slotIndex) const							{ return *(getSlotDataPtr(slotIndex)); }								// Get the size of the slot.
	inline int getPrevSlot(int slotIndex) const							{ return *(getSlotDataPtr(slotIndex) + 1); }							// Get the prev open slot.
	inline int getNextSlot(int slotIndex) const							{ return *(getSlotDataPtr(slotIndex) + 2); }							// Get the next open slot.
	inline void setSlotSize(int slotIndex, int size)					{ *(getSlotDataPtr(slotIndex)) = size; }								// Set the size of the slot.
	inline void setPrevSlot(int slotIndex, int prevSlot)				{ *(getSlotDataPtr(slotIndex) + 1) = prevSlot; }						// Set the prev open slot.
	inline void setNextSlot(int slotIndex, int nextSlot)				{ *(getSlotDataPtr(slotIndex) + 2) = nextSlot; }						// Set the next open slot.
	inline void increaseSlotSize(int slotIndex, int size)				{ setSlotSize(slotIndex, getSlotSize(slotIndex) + size); }				// Increase the slot size.
	inline bool hasNextSlot(int slotIndex) const						{ return getNextSlot(slotIndex) != -1; }								// Check if the given slot has a next slot.
	inline bool hasPrevSlot(int slotIndex) const						{ return getPrevSlot(slotIndex) != -1; }								// Check if the given slot has a previous slot.
	inline bool firstFreeSlotValid() const								{ return slotIsValid(m_firstFreeSlot); }								// Checks if the first free slot is valid (exists).
	inline bool lastFreeSlotValid() const								{ return slotIsValid(m_lastFreeSlot); }									// Checks if the last free slot is valid (exists).
	inline bool slotIsValid(int slot) const								{ return slot != -1; }													// Checks if the slot is valid.
    inline bool slotsAreAdjacent(int firstSlot, int secondSlot) const	{ return firstSlot + getSlotSize(firstSlot) == secondSlot; }			// Checks if the slots are adjacent to each other.
	inline bool isLastSlotAtEnd() const									{ return slotsAreAdjacent(m_lastFreeSlot, m_capacity); }				// Checks if the last slot in the list sits at the end of memory.
	inline bool isSlotContained(int parent, int child) const			{ return isSlotContained(parent, getSlotSize(parent), child); }			// Checks if the child slot is contained in the parent.
	inline bool isSlotContained(int parent, int parentSize, int child) const { return child >= parent && child < parent + parentSize; }			// Checks if the child slot is contained in the parent, based on the parent size.
	inline void connectSlots(int firstSlot, int secondSlot)				{ setNextSlot(firstSlot, secondSlot); setPrevSlot(secondSlot, firstSlot); }	// Connect the two slots via their next and prev slots.

	// Data.
	T* m_data = nullptr;			                      // Pointer to the data on the heap.
	IteratorMode m_iteratorMode = IteratorMode::ELEMENTS; // The mode of iteration.
	int m_capacity = 0;				                      // The total elements that can be held.
	int m_elementCount = 0;			                      // Keep track of the amount of elements allocated.
	int m_sizeOfElement = sizeof(T);                      // The size (in bytes) of each open slot (each slot can hold one element).
	int m_firstFreeSlot = -1;		                      // The indices of the slots,
	int m_lastFreeSlot  = -1;		                      // -1 = no free slot.
	int m_capacityIncrements = 10;	                      // The amount of slots added/removed on resize.
	float m_resizeThreshold = 1.1f;                       // The ratio that the freelist has to be reduced beyond the resize in increments to be reduced in size.
									                      // This prevents the situation where resizes happens unneccesarily.

public:

	// ----------------- //
	//  I T E R A T O R  //
	// ----------------- //

	// FreeLists need a custom iterator, since we want to be able to avoid unused 
	// slots and set the mode of iteration.
	template<typename FreeListType>
	class IteratorBase
	{
	public:

		// Contructor.
		IteratorBase(FreeListType* fl, int index, int nextFreeSlot, IteratorMode mode)
		{
			m_freeList = fl;
			m_index = index;
			m_nextFreeSlot = nextFreeSlot;
			m_iteratorMode = mode;
			if(m_iteratorMode == IteratorMode::MEMORY)
				updateElementsInMemory();
		}

		// Destructor.
		virtual ~IteratorBase() = default;

		// Update the amount of elements in the current contiguous region.
		inline void updateElementsInMemory()
		{
			// Set the memory region size.
			if (m_freeList->slotIsValid(m_nextFreeSlot)) m_elementsInMemoryRegion = m_nextFreeSlot - m_index;
			else								 	     m_elementsInMemoryRegion = m_freeList->capacity() - m_index;
		}

		// Operators.
		inline void operator++()
		{
			switch (m_iteratorMode)
			{
			case IteratorMode::MEMORY:
				// Move to next memory region.
				if(m_freeList->slotIsValid(m_nextFreeSlot))
				{
				 	m_index = m_freeList->getSlotSize(m_nextFreeSlot);
				}
				// No memory regions left.
				else 
				{
					m_index = m_freeList->capacity();
					return;
				}
				// Upadte next slot.
				m_nextFreeSlot = m_freeList->getNextSlot(m_nextFreeSlot);
				updateElementsInMemory();
				break;

			case IteratorMode::ELEMENTS:
				// Next slot contains data.
				if (m_nextFreeSlot == -1 || m_index + 1 < m_nextFreeSlot)
				{
					m_index++;
				}
				// 'Jump' over linked list slot.
				else
				{
					m_index += m_freeList->getSlotSize(m_nextFreeSlot) + 1;
					m_nextFreeSlot = m_freeList->getNextSlot(m_nextFreeSlot);
				}
				break;

			default:
				assert(false);  // Invalid iterator mode.
				break;
			}
		}
		
		inline bool operator!=(const IteratorBase<FreeListType>& other) const { return m_index != other.m_index; }

		// Data.
		int m_index = NULL;										// The index in memory of the current element.
		int m_nextFreeSlot = NULL;								// The index of the next slot that does not contain data.
		int m_elementsInMemoryRegion = NULL;					// The amount of elements in the current memory region.
		FreeListType* m_freeList = nullptr;						// FreeList iterating over.
		IteratorMode m_iteratorMode = IteratorMode::ELEMENTS;	// The set iteration mode.
	};

	// ------------------------------------- //
	//  N O N   C O N S T   I T E R A T O R  //
	// ------------------------------------- //

	class Iterator : public IteratorBase<FreeList<T>>
	{
	public:

		// Constructor.
		Iterator(FreeList<T>* fl, int index, int nextFreeSlot, IteratorMode mode) 
			: IteratorBase<FreeList<T>>(fl, index, nextFreeSlot, mode)
		{ }	

		// Operators.
		inline const T& operator*() const { return (*this->m_freeList)[this->m_index]; }
		inline T& operator*()             { return (*this->m_freeList)[this->m_index]; }	
	};

	// Iterator functions.
	inline Iterator begin()
	{ 
		// First slot in memory has a linked list entry.
		if (m_firstFreeSlot == 0) return Iterator(this, getSlotSize(m_firstFreeSlot), getNextSlot(m_firstFreeSlot), m_iteratorMode);
		// Construct iterator normally.
		else					  return Iterator(this, 0, m_firstFreeSlot, m_iteratorMode);
	}

	inline Iterator end()
	{ 
		return Iterator(this, capacity(), -1, m_iteratorMode); 
	}

	// ----------------------------- //
	//  C O N S T   I T E R A T O R  //
	// ----------------------------- //

	class ConstIterator : public IteratorBase<const FreeList<T>>
	{
	public:

		// Constructor.
		ConstIterator(const FreeList<T>* fl, int index, int nextFreeSlot, IteratorMode mode) 
			: IteratorBase<const FreeList<T>>(fl, index, nextFreeSlot, mode)
		{ }

		// Operators.
		inline const T& operator*() const { return (*this->m_freeList)[this->m_index]; }
	};

	// Const Iterator functions.
	inline const ConstIterator begin() const
	{ 
		// First slot in memory has a linked list entry.
		if (m_firstFreeSlot == 0) return ConstIterator(this, getSlotSize(m_firstFreeSlot), getNextSlot(m_firstFreeSlot), m_iteratorMode);
		// Construct iterator normally.
		else					  return ConstIterator(this, 0, m_firstFreeSlot, m_iteratorMode);
	}
	inline const ConstIterator end() const
	{ 
		return ConstIterator(this, capacity(), -1, m_iteratorMode); 
	}
};
