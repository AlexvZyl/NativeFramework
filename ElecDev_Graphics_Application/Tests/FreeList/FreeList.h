// Contains the data and methods that are shared between SEFreeList and DEFreeList.

#define FREELIST_MAX_CAPACITY 2147483648 // 2^31 for int.

template<typename T>
class FreeList
{
public:

    // The modes of iteration.
	enum class IteratorMode { ELEMENTS, MEMORY };

	// Constructors.
	inline FreeList() 
    {
        
    }
    
	inline FreeList(int capacity)
		: FreeList()
	{
		queryResize(capacity);
	}
	inline FreeList(int capacity, int capacityIncrements)
		: FreeList()
	{
		setCapacityIncrements(capacityIncrements);
		queryResize(capacity);
	}

	// Copy constructor.
	inline FreeList(const FreeList& other)
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
	inline ~FreeList() 
	{
		if (m_data)
		{
			callAllDestructors();
			free(m_data);
		}
	}

protected:

};