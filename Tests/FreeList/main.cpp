#include <iostream>
#include <string>
#undef _DEBUG
#include "FreeList.h"
#include "Profiler.h"
#include <vector>
#include <Windows.h>

class TestClass
{
public:
    inline TestClass() { constructorCalls++; }
    inline ~TestClass() { destructorCalls++; }
    TestClass(const TestClass& other)
    { 
        copyConstCalls++; 
        index = other.index;
        data2 = other.data2;
        data3 = other.data3;
        myString = other.myString;
    }
    inline static void reset()
    {
        constructorCalls = 0;
        destructorCalls = 0;
        copyConstCalls = 0;
    }
    inline void myFunction() { totalCalls++; }
    // Local data.
    std::string myString = "Short string.";
    unsigned index = 0;
    unsigned data2 = 0;
    unsigned data3 = 0;
    // Static data.
    inline static int totalCalls = 0;
    inline static int constructorCalls = 0;
    inline static int destructorCalls = 0;
    inline static int copyConstCalls = 0;
};

inline void loop(FreeListBase<TestClass>& fl) 
{
    for (auto& entry : fl) 
        std::cout << "Index: " << entry.index << ", String: " << entry.myString << "\n";
    std::cout << "\n";
}

inline void emplace(FreeListBase<TestClass>& fl)
{
    int index = fl.emplace();
    fl[index].index = index;
}

inline void printCallData()
{
    std::cout << "[Constructor Calls] : [" << TestClass::constructorCalls << "]\n";
    std::cout << "[Destructor Calls] : [" << TestClass::destructorCalls << "]\n";
    std::cout << "[Copy Constructor Calls] : [" << TestClass::copyConstCalls << "]\n\n";
    TestClass::reset();
}

inline void printMetaData(FreeListBase<TestClass>& freeList)
{
    std::cout << "[Capacity] : [" << freeList.capacity() << "]\n";
    std::cout << "[Allocated] : [" << freeList.allocated() << "]\n";
    std::cout << "[Count] : [" << freeList.count() << "]\n";
    std::cout << "[Size] : [" << freeList.size() << "]\n";
    std::cout << "[First Free Slot] : [" << freeList.getFirstFreeSlot() << "]\n";
    std::cout << "[Last Free Slot] : [" << freeList.getLastFreeSlot() << "]\n\n";
}

inline void tooLargeArrayLastFreeSlot(FreeListBase<TestClass>& freeList)
{
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    printMetaData(freeList);
    loop(freeList);
    freeList.erase(8,2);
    TestClass myvec[3] = { TestClass(), TestClass(), TestClass() };
    freeList.push(myvec, 3);
    printMetaData(freeList);
    loop(freeList);
    printCallData();
}

inline void simpleEmplaceErase(FreeListBase<TestClass>& freeList)
{
    std::cout << "Simple Emplace Erase\n\n";

    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    printMetaData(freeList);
    loop(freeList);
    freeList.erase(0);
    freeList.erase(1);
    freeList.erase(2);
    freeList.erase(3);
    freeList.erase(4);
    printMetaData(freeList);
    loop(freeList);
    printCallData();
}

inline void simplePushErase(FreeListBase<TestClass>& freeList)
{
    std::cout << "Simple Push Erase\n\n";

    freeList.push(TestClass());
    freeList.push(TestClass());
    freeList.push(TestClass());
    freeList.push(TestClass());
    freeList.push(TestClass());
    printMetaData(freeList);
    loop(freeList);
    freeList.erase(0);
    freeList.erase(1);
    freeList.erase(2);
    freeList.erase(3);
    freeList.erase(4);
    printMetaData(freeList);
    loop(freeList);
    printCallData();
}

inline void centerRemovePushArray(FreeListBase<TestClass>& freeList)
{
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    printMetaData(freeList);
    loop(freeList);
    freeList.erase(4,3);
    TestClass myvec[3] = { TestClass(), TestClass(), TestClass() };
    freeList.push(myvec, 3);
    printMetaData(freeList);
    loop(freeList);
    printCallData();
}

inline void centerRemovePushTooLargeArray(FreeListBase<TestClass>& freeList)
{
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    printMetaData(freeList);
    loop(freeList);
    freeList.erase(4,3);
    TestClass myvec[4] = { TestClass(), TestClass(), TestClass(), TestClass() };
    freeList.push(myvec, 4);
    printMetaData(freeList);
    loop(freeList);
    printCallData();
}

inline void performanceTests()
{
    std::cout << "\n";
    const int TOTAL_LOOPS = 10000;
    const int RESERVE = TOTAL_LOOPS / 5;

  {
        PROFILE_SCOPE("Lumen::Freelist Total");
        FreeList<TestClass> myList(0, RESERVE);

        {
            PROFILE_SCOPE("Lumen::Freelist Emplacing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myList.emplace();
            }
        }

        {
            PROFILE_SCOPE("Lumen::Freelist Copying & Destroying");
            FreeList<TestClass> copiedList = myList;
        }

        {
            PROFILE_SCOPE("Lumen::Freelist Element Indexing");
            myList.iterateElements();
            for(auto& entry : myList)
            {
                entry.myFunction();
            }
        }

        std::cout << "[Lumen::Freelist Total Calls] : [" << TestClass::totalCalls << "]\n";
        TestClass::totalCalls = 0;

        {
            PROFILE_SCOPE("Lumen::Freelist Memory Indexing");
            myList.iterateMemory();
            int fragments = 0;
            for(auto& entry : myList)
            {
                TestClass* memory = &entry;
                fragments++;
            }
        }

        {
            PROFILE_SCOPE("Lumen::Freelist Erasing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myList.erase(i);
            }
        }
    }

    std::cout << "\n";

    {
        PROFILE_SCOPE("std::vector Total");
        std::vector<TestClass> myVec;

        {
            PROFILE_SCOPE("std::vector Emplacing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myVec.emplace_back();
            }
        }

        {
            PROFILE_SCOPE("std::vector Copying & Destroying");
            std::vector<TestClass> copiedVec = myVec;
        }

        {
            PROFILE_SCOPE("std::vector Element Indexing");
            for(auto& entry : myVec)
            {
                entry.myFunction();
            }
        }

        std::cout << "[std::vector Total Calls] : [" << TestClass::totalCalls << "]\n";
        TestClass::totalCalls = 0;

        {
            PROFILE_SCOPE("std::vector Memory Indexing");
            TestClass* memory = myVec.data();
        }

        {
            PROFILE_SCOPE("std::vector Erasing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myVec.erase(myVec.begin());
            }
        }
    }

    TestClass::reset();
}

int main(int, char**)
{
    // DE FreeList.
    FreeList<TestClass> testList(0, 5);
    // simpleEmplaceErase(testList);
    // simplePushErase(testList);
    // centerRemovePushArray(testList);
    // centerRemovePushTooLargeArray(testList);
    tooLargeArrayLastFreeSlot(testList);

    // SE FreeList.
    SEFreeList<TestClass> testList2(0, 5);
    // simpleEmplaceErase(testList2);
    // simplePushErase(testList2);
    // centerRemovePushArray(testList2);
    // centerRemovePushTooLargeArray(testList2);
    tooLargeArrayLastFreeSlot(testList2);

    
    // Performance.
    // performanceTests();

    return 0;
}