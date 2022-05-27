#pragma once

#include <tuple>
#include <assert.h>
#include <stdint.h>

// When there is a slot in memory that is not being used by the FreeList, 
// it is used as a member in the FreeList. 
// Ordering the memory leads to less fragmentation.
// 8 Bytes is used as the minimum size because the freelist has to support 2 UInts or 2 Floats,
// since this a common data type in graphics.
// Since slot sizes are capped (at 2^16-1), the freelist has to support packing slots.
// Memory outline of each slot, given in bytes:
//
//	-----------------------------------------------------------------
//	|				|				|				|				|
//	|		2		|		2		|		2		|		2		|
//	|	 [Size]		|  [Prev slot]	|  [Next slot]	|	[Layout]	|
//	-----------------------------------------------------------------
//  
// Size: The size of the current slot.    
// Next slot: The index if the following slot.
// Prev slot: The index if the previous slot.
// Memory: Describes how the slot sits in memory (Packed?  Alone?)
//		   See the MemoryLayout enumerator.

#define FREELIST_MINIMUM_ELEMENT_SIZE 8 	// Bytes.
#define FREELIST_MAX_CAPACITY 4294967296	// 2^32
#define FREELIST_MAX_SLOT_SIZE 65535 		// 2^16 - 1 (Since -1 is used to identify an invalid slot)

template<typename T>
class FreeList
{
private:
	// Memory type used in the slots.
	typedef uint16_t u16;
	// Used to describe how the slot sits in memory.
	enum class MemoryLayout : u16
	{
		NONE,
		SINGLE,
		LEFT_PACKED,
		RIGHT_PACKED,
		PACKED
	};
public:

	// The modes of iteration.
	enum class IteratorMode { ELEMENTS, MEMORY };

	// Constructors.
	FreeList()
	{
		// Ensure element size is large enough.
		static_assert(sizeof(T) >= FREELIST_MINIMUM_ELEMENT_SIZE, "The FreeList element must be larger than or equal to 8 bytes.");
		m_sizeOfElement = sizeof(T);
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
	// Destructors are called.
	inline void clear()
	{
		callAllDestructors();
		m_firstFreeSlot = 0;
		m_lastFreeSlot = 0;
		m_elementCount = 0;
		splitIntoSlotsAndMerge(0, m_capacity, -1, -1);
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

	// Push element in the next open slot.
	// Returns the index of the slot start.
	inline int push(T& element)
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
			new (getSlotElementPtr(slotIndex + i)) T(*(elementPtr + i));

		// Return the index of the first element.
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
			new (getSlotElementPtr(slotIndex + i)) T(*(elementPtr + i));

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
		assert(index < m_capacity); // Trying to pop outside of capacity.
		slotDestructor(index);
		freeSlot(index);
	}

	// Pop the range of elements.
	inline void erase(int startIndex, int size)
	{
		assert(startIndex + size < m_capacity); // Trying to pop outside of capacity.
		for (int i = startIndex; i < startIndex + size; i++)
			slotDestructor(i);
		freeSlot(startIndex, size);
	}

	// Utilities.
	inline constexpr int capacity() { return m_capacity; }					// Get the capacity in amount of elements.
	inline constexpr int allocated() { return capacity() * m_sizeOfElement; }// Get the amount of memory used in bytes.
	inline constexpr int count() { return m_elementCount; }				// Get the amount of elements comitted.
	inline constexpr int size() { return count() * m_sizeOfElement; }	// Get the amount of data used (in bytes).
	inline void setCapacityIncrements(int increments) { m_capacityIncrements = increments; }	// Set the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline int getCapacityIncrements() { return m_capacityIncrements; }		// Get the amount of elements that the FreeLists resizes on a resize.  This prevents resizing every time an element is added.
	inline void setResizeThreshold(float threshold) { m_resizeThreshold = threshold; }		// Set the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline float getResizeThreshold() { return m_resizeThreshold; }			// Get the threshold for resizing (more specifically, decreasing).  Prevents unnceccesary allocation and copying of data.
	inline void setIteratorMode(IteratorMode mode) { m_iteratorMode = mode; }				// Set the mode of iteration.
	inline IteratorMode getIteratorMode() { return m_iteratorMode; }				// Get the mode of iteration.

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

	// Check if a resize has to occur based on the change.
	// Return true if resize occured.
	inline bool queryResize(int elementChange)
	{
		// Ensure actual resize query.
		if (!elementChange) return false;

		// Calculate the expected element count with query.
		int newElementCount = elementChange + m_elementCount;

		// Increase.
		if ((elementChange > 0) && newElementCount > m_capacity)
		{
			int toIncrease = (((newElementCount - m_capacity) / m_capacityIncrements) + 1) * m_capacityIncrements;
			allocate(m_capacity + toIncrease);
			return true;
		}

		// Decrease.
		else if ((newElementCount >= 0) && newElementCount < m_capacity - (m_capacityIncrements * m_resizeThreshold))
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
		assert(newCapacity < FREELIST_MAX_CAPACITY); // Exceeded FreeList maximum capacity.

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
		else assert(false); // Error with memory allocation.

		// Update capacity.
		m_capacity = newCapacity;
	}

	// Find the first slot that fits the data.
	inline int findSlotFirstFit(int slotSize = 1)
	{
		// Find an open slot that is large enough, starting from the first free one.
		// If there is no valid first slot, skip the initial search, resize and try again.
		int candidateSlotIndex = m_firstFreeSlot;
		if (slotIsValid(candidateSlotIndex))
		{
			// Get the current slot data.
			int searchSlotIndex = candidateSlotIndex;
			int currentRegionSize = 0;

			// Keep looking until a large enough memory region is found.
			while (currentRegionSize < slotSize)
			{
				// Increment size and move to next slot.
				currentRegionSize += getSlotSize(searchSlotIndex);
				searchSlotIndex = getNextSlot(searchSlotIndex);

				// The next slot does not exist. 
				if (!isSlotValid(searchSlotIndex))
				{
					// If it is not large enough no region was found.
					if (currentRegionSize < size) candidateSlotIndex = -1;
					break;
				}

				// Check if this is the last slot in the region.
				MemoryLayout layout = getLayout(searchSlotIndex);
				if ((layout != MemoryLayout::LEFT_PACKED) && (layout != MemoryLayout::PACKED))
				{
					// Region is not large enough.
					if (currentRegionSize < size)
					{
						currentRegionSize = 0;
						candidateSlotIndex = searchSlotIndex;
					}
					// Found a valid region.
					else break;
				}

				// If it is not the last slot just keep searching.
			}

			// Found a valid slot.
			if (slotIsValid(candidateSlotIndex)) return candidateSlotIndex;
		}

		// Did not find a valid slot.  Resize and search again.
		else if (queryResize(slotSize)) return findSlotFirstFit(slotSize);

		// If the resize cannot occur we ran out of memory...
		assert(false); // Ran out of memory.
		return 0;
	}

	// Commit a slot to be used.
	// This does not check any validity.  Make sure find is called before comitting!
	inline void commitSlot(int slotIndex, int size = 1)
	{
		// Comitting a slot means the elements are added.
		m_elementCount += size;

		// Get the slot data.
		auto [slotSize, prevSlot, nextSlot, layout] = getSlotData(slotIndex);
		bool curHasPrevSlot = hasPrevSlot(slotIndex);
		bool curHasNextSlot = hasNextSlot(slotIndex);

		// Only a single slot will be affected.
		if (slotSize > size)
		{
			// The next open slot is now the new slot.
			setSlotData(slotIndex + size, slotSize - size, nextSlot, prevSlot);

			// Update prev slot data.
			// These slots can never be merged so no need to check it.
			if (curHasPrevSlot)
			{
				setNextSlot(prevSlot, slotIndex + size);
			}
			else m_firstFreeSlot = slotIndex + size;

			// Update next slot data.
			if (curHasNextSlot)
			{
				setPrevSlot(nextSlot, slotIndex + size);
				// They can merge in the case where they are adjacent and now small enough.
				attemptMerge(slotIndex + size, nextSlot);
			}
			else m_lastFreeSlot = slotIndex + size;
		}

		// In this case more than one slot is being affected.
		else
		{
			int currentSlotIndex = slotIndex;
			int nextSlotIndex = getNextSlot(currentSlotIndex)
				// Find the slot that will be fragmented.
				while (nextSlotIndex < slotIndex + size)
				{
					currentSlotIndex = nextSlotIndex;
					nextSlotIndex = getNextSlot(currentSlotIndex);
				}

			// If the found slot sits at +1 we can simply discard the previous slots
			// by updating prevSlot.
			if (nextSlotIndex - 1 == slotIndex + size)
			{
				if (curHasPrevSlot) setPrevSlot(nextSlotIndex, prevSlot);
				else  			   m_firstFreeSlot = nextSlotIndex;
			}

			// The slot has to be fragmented.
			else
			{
				u16 newSize = nextSlotIndex - (slotIndex + size);
				setSlotData(slotIndex + size, newSize, prevSlot, nextSlotIndex);
			}
		}
	}

	// Free a used slot.
	inline void freeSlot(int slotIndex, int size = 1)
	{
		// Freeing a slot means an element has been removed.
		m_elementCount -= size;
		assert(m_elementCount >= 0); // Tried to free too many elements.

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
	// Automatically determines the slot memory layout.
	inline void setSlotData(int slotIndex, u16 slotSize, u16 prevSlot, u16 nextSlot)
	{
		u16* data = getSlotDataPtr(slotIndex);
		*(data++) = slotSize;
		*(data++) = prevSlot;
		*(data++) = nextSlot;
		*(data) = getLayout(slotIndex, prevSlot, nextSlot);
	}

	// Get the data describing the slot.
	// { Size, PrevSlot, NextSlot, Layout }
	inline std::tuple<u16, u16, u16, MemoryLayout> getSlotData(int slotIndex)
	{
		u16* data = getSlotDataPtr(slotIndex);
		return { *(data), *(data + 1), *(data + 2),  *(data + 3) };
	}

	// Updated the tracked free slots.
	inline void updateFreeSlots(int slotIndex)
	{
		if (slotIndex < m_firstFreeSlot || !firstFreeSlotValid()) m_firstFreeSlot = slotIndex;
		if (slotIndex > m_lastFreeSlot || !lastFreeSlotValid())  m_lastFreeSlot = slotIndex;
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
		if (!hasValidFirst && !hasValidLast) {}

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
			else if (distanceFromFirst <= distanceFromEnd)
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

	// Checks if a merge is possible based on their sizes, adjacency and validity.
	inline void attemptMerge(int firstSlot, int secondSlot)
	{
		// Ensure slots are valid.
		if (!isSlotValid(firstSlot) || !isSlotValid(secondSlot)) return;

		// Ensure slots merge do not exceed maximum capacity.
		if (getSlotSize(firstSlot) + getSlotSize(secondSlot) > FREELIST_MAX_SLOT_SIZE) return;

		// Ensure slots are adjacent.
		if (!slotsAreAdjacent(firstSlot, secondSlot)) return;

		// Merge is possible.
		merge(firstSlot, secondSlot);
	}

	// TODO: Implement function that selects the best merging operation based on the
	// adjacent slots.  This can lead to reduced fragmentation.
	inline void attemptMerge(int prevSlot, int middleSlot, int nextSlot)
	{

	}

	// Merge the two slots.  Does not check if merge is valid.
	// If unsure rather call attemptMerge() instead.
	inline void merge(int firstSlot, int secondSlot)
	{
		increaseSlotSize(firstSlot, getSlotSize(secondSlot));
		setNextSlot(firstSlot, getNextSlot(secondSlot));
		if (hasNextSlot(secondSlot)) setPrevSlot(getNextSlot(secondSlot), firstSlot);
		if (m_lastFreeSlot == secondSlot) m_lastFreeSlot = firstSlot;
		updateSlotLayout(firstSlot);
	}

	// Takes a starting index and splits it onto slots, given the size of the region.
	// Provide the adjacent slots if this function is called between other slots.
	// Also tries to merge the memory with the prev and next slot.
	inline void splitIntoSlotsAndMerge(int startIndex, int size, u16 prevSlot, u16 nextSlot)
	{
		// One slot can take the entire region.
		if (size < FREELIST_MAX_SLOT_SIZE)
		{
			setSlotData(startIndex, size, prevSlot, nextSlot);
			updateFreeSlots(startIndex);
			// TODO: Possible optimisation.
			// Depending on which one fits better we can reduce fragmentation here.
			attemptMerge(prevSlot, startIndex);
			attemptMerge(startIndex, nextSlot);
			return;
		}

		// The region has to be divided.
		int currentSize = size;
		int currentSlot = startIndex;
		int currentPrevSlot = prevSlot;
		// Update the prev slot data.
		if (isValidSlot(prevSlot))
		{
			setNextSlot(prevSlot, startIndex);
			updateSlotLayout(prevSlot);
		}
		// Keep allocating while there is memory left.
		while (currentSize)
		{
			// Still need more than one slot.
			if (currentSize > FREELIST_MAX_SLOT_SIZE)
			{
				setSlotData(currentSlot, FREELIST_MAX_SLOT_SIZE, currentPrevSlot, currentSlot + FREELIST_MAX_SLOT_SIZE);
				updateFreeSlots(currentSlot);
				currentPrevSlot = currentSlot;
				currentSlot += FREELIST_MAX_SLOT_SIZE;
				currentSize -= FREELIST_MAX_SLOT_SIZE;
			}
			// Last memory can be fit into one slot.
			else
			{
				setSlotData(currentSlot, currentSize, currentPrevSlot, nextSlot)
					updateFreeSlots(currentSlot);
				attemptMerge(currentSlot, nextSlot);
				currentSize = 0;
			}
		}
	}

	// Get the layout description based on the adjacent slots.
	inline MemoryLayout getLayout(int slotIndex, int prevSlot, int nextSlot)
	{
		// Check how the slot is packed.
		bool leftPacked = isSlotValid(prevSlot) && slotsAreAdjacent(prevSlot, slotIndex);
		bool rightPacked = isSlotValid(nextSlot) && slotsAreAdjacent(slotIndex, nextSlot);
		// Determine the layout.
		if (leftPacked && rightPacked) return MemoryLayout::PACKED;
		else if (leftPacked) 	   	  return MemoryLayout::LEFT_PACKED;
		else if (rightPacked) 	      return MemoryLayout::RIGHT_PACKED;
		else 					      return MemoryLayout::SINGLE;
	}

	// Slot utilities.
	inline u16* getSlotDataPtr(int slotIndex) { return reinterpret_cast<u16*>(m_data + slotIndex); }					// Get a pointer to the slot data.
	inline u16 getSlotSize(int slotIndex) { return *(getSlotDataPtr(slotIndex)); }								// Get the size of the slot.
	inline u16 getPrevSlot(int slotIndex) { return *(getSlotDataPtr(slotIndex) + 1); }							// Get the prev open slot.
	inline u16 getNextSlot(int slotIndex) { return *(getSlotDataPtr(slotIndex) + 2); }							// Get the next open slot.
	inline MemoryLayout getSlotLayout(int slotIndex) { return *(getSlotDataPtr(slotIndex) + 3); }							// Get the memory layout of the slot.
	inline void setSlotSize(int slotIndex, u16 size) { *(getSlotDataPtr(slotIndex)) = size; }								// Set the size of the slot.
	inline void setPrevSlot(int slotIndex, u16 prevSlot) { *(getSlotDataPtr(slotIndex) + 1) = prevSlot; }						// Set the prev open slot.
	inline void setNextSlot(int slotIndex, u16 nextSlot) { *(getSlotDataPtr(slotIndex) + 2) = nextSlot; }						// Set the next open slot.
	inline void setSlotLayout(int slotIndex, MemoryLayout layout) { *(getSlotDataPtr(slotIndex) + 3) = layout; }							// Set the layout of the slot.
	inline bool hasNextSlot(int slotIndex) { return getNextSlot(slotIndex) != -1; }								// Check if the given slot has a next slot.
	inline bool hasPrevSlot(int slotIndex) { return getPrevSlot(slotIndex) != -1; }								// Check if the given slot has a previous slot.
	inline T* getSlotElementPtr(int slotIndex) { return m_data + slotIndex; }											// Get a pointer to the slot as an element.
	inline bool firstFreeSlotValid() { return m_firstFreeSlot != -1; }										// Checks if the first free slot is valid (exists).
	inline bool lastFreeSlotValid() { return m_lastFreeSlot != -1; }										// Checks if the last free slot is valid (exists).
	inline bool slotIsValid(int slot) { return slot != -1; }													// Checks if the slot is valid.
	inline void slotDestructor(int slotIndex) { getSlotElementPtr(slotIndex)->~T(); }									// Call the destructor of the element in the slot.
	inline void increaseSlotSize(int slotIndex, u16 size) { setSlotSize(slotIndex, getSlotSize(slotIndex) + size); }				// Increase the slot size.
	inline bool slotsAreAdjacent(int firstSlot, int secondSlot) { return firstSlot + getSlotSize(firstSlot) == secondSlot; }			// Checks if the slots are adjacent to each other.
	inline void updateSlotLayout(int slotIndex) { setSlotLayout(getLayout(slotIndex, getPrevSlot(slotIndex), getNextSlot(slotIndex))); }  // Upadte the layout description of the slot, based on current data.

	// Memory.
	inline void copyToSlot(T* source, int slotIndex) { memcpy(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Copy the element data into the slot.
	inline void copyToSlot(const T* source, int slotIndex) { memcpy(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Copy the element data into the slot.
	inline void moveToSlot(T* source, int slotIndex) { memmove(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Move the element data into the slot.
	inline void moveToSlot(const T* source, int slotIndex) { memmove(getSlotElementPtr(slotIndex), source, m_sizeOfElement); }		// Move the element data into the slot.
	inline void setSlotsMemory(int slotIndex, u16 size, int val = 0xCC) { memset(getSlotElementPtr(slotIndex), val, m_sizeOfElement * size); }	// Set the value of the slots memory.
	inline void setSlotMemory(int slotIndex, int val = 0xCC) { setSlotsMemory(slotIndex, 1, val); }									// Set the value of the slot memory.

	// Data.
	T* m_data = nullptr;			// Pointer to the data on the heap.
	int m_capacity = 0;				// The total elements that can be held.
	int m_elementCount = 0;			// Keep track of the amount of elements allocated.
	int m_sizeOfElement = NULL;		// The size (in bytes) of each open slot (each slot can hold one element).
	int m_firstFreeSlot = -1;		// The indices of the slots,
	int m_lastFreeSlot = -1;		// -1 = no free slot.
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
				// Invalid iterator mode.
				assert(false);
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