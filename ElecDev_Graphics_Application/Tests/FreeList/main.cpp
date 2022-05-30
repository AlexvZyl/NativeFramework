#include <iostream>
#include <string>
#undef _DEBUG
#include "FreeList.h"
#include "Profiler.h"
#include <vector>
#include <Windows.h>

class Test 
{
public:
    Test() 
    {
    }
    ~Test() 
    {
    }

    inline void myFunction() { totalCalls++; }

    unsigned index = 0;
    unsigned data2 = 0;
    unsigned data3 = 0;
    inline static int totalCalls = 0;
    std::string myString = "Testing for data corruption yay.";
};
inline void loop(FreeList<Test>& fl) 
{
    std::cout << "\nCapacity: " << fl.capacity() << ".\n";
    std::cout << "Allocated: " << fl.allocated() << ".\n";
    std::cout << "Count: " << fl.count() << ".\n";
    std::cout << "Size: " << fl.size() << ".\n\n";
    for (auto& entry : fl)
        std::cout << "Index: " << entry.index << ", String: " << entry.myString << "\n";
    std::cout.flush();
}
inline void emplace(FreeList<Test>& fl)
{
    int index = fl.emplace();
    fl[index].index = index;
}

int main(int, char**)
{
    std::cout << "\n";
    const int TOTAL_LOOPS = 10000;
    const int RESERVE = TOTAL_LOOPS / 5;

    {
        PROFILE_SCOPE("Lumen::Freelist Total");
        FreeList<Test> myList(0, RESERVE);

        {
            PROFILE_SCOPE("Lumen::Freelist Emplacing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myList.emplace();
            }
        }

        {
            PROFILE_SCOPE("Lumen::Freelist Copying & Destroying");
            FreeList<Test> copiedList = myList;
        }

        {
            PROFILE_SCOPE("Lumen::Freelist Element Indexing");
            myList.iterateElements();
            for(auto& entry : myList)
            {
                entry.myFunction();
            }
        }

        std::cout << "[Lumen::Freelist Total Calls] : [" << Test::totalCalls << "]\n";
        Test::totalCalls = 0;

        {
            PROFILE_SCOPE("Lumen::Freelist Memory Indexing");
            myList.iterateMemory();
            int fragments = 0;
            for(auto& entry : myList)
            {
                Test* memory = &entry;
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
        std::vector<Test> myVec;

        {
            PROFILE_SCOPE("std::vector Emplacing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myVec.emplace_back();
            }
        }

        {
            PROFILE_SCOPE("std::vector Copying & Destroying");
            std::vector<Test> copiedVec = myVec;
        }

        {
            PROFILE_SCOPE("std::vector Element Indexing");
            for(auto& entry : myVec)
            {
                entry.myFunction();
            }
        }

        std::cout << "[std::vector Total Calls] : [" << Test::totalCalls << "]\n";
        Test::totalCalls = 0;

        {
            PROFILE_SCOPE("std::vector Memory Indexing");
            Test* memory = myVec.data();
        }

        {
            PROFILE_SCOPE("std::vector Erasing");
            for(int i = 0; i<TOTAL_LOOPS; i++)
            {
                myVec.erase(myVec.begin());
            }
        }
    }

    return 0;
}