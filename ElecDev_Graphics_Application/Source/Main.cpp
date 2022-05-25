#include "Utilities/Platform/Preprocessor.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"

// Testing.
#include "Utilities/Memory/FreeList.h"
#include "Utilities/Logger/Logger.h"

struct TestStruct 
{
    ~TestStruct() 
    {
        std::cout << "TestStruct::Destructor()\n";
    }
};

class TestClass
{
public:

    TestClass() 
    {
        std::cout << "TestClass::Constructor()\n";
        myUPtrString = std::make_unique<std::string>("This is my UPtr String yay!");
        myStruct = std::make_unique<TestStruct>();
    }

    ~TestClass() 
    {
        std::cout << "TestClass::Destructor()\n";
    }

    inline void myFunction() 
    {
        std::cout << "TestClass::myFunction(), Index: " << std::to_string(indexInList) << ", String: " << myString.c_str() << ", UPtr String: " << myUPtrString->c_str() << "\n";
    }

    // Data.
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    int indexInList = 0;
    std::string myString = "This is my very long string so that I can debug yay";
    std::unique_ptr<std::string> myUPtrString = nullptr;
    std::unique_ptr<TestStruct> myStruct;
};

void push(FreeList<TestClass>& fl) 
{
    int index = fl.push({});
    fl[index].indexInList = index;
    std::cout << "Pushed Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void emplace(FreeList<TestClass>& fl)
{
    int index = fl.emplace();
    fl[index].indexInList = index;
    std::cout << "Emplaced Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void pop(FreeList<TestClass>& fl, int index)
{
    fl.pop(index);
    std::cout << "Popped Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

int main(int, char**)
{
    // TESTING!
    FreeList<TestClass> freeList(0, 5);
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
    pop(freeList, 3);
    pop(freeList, 6);
    pop(freeList, 9);

    // Iteration.
    for (auto& entry : freeList) entry.myFunction();

    emplace(freeList);
    emplace(freeList);
    emplace(freeList);

    // Iteration.
    for (auto& entry : freeList) entry.myFunction();

    // Initialisation.
    Application application;
    Renderer::initialise();
    
    std::vector<TestClass> myVec;
    myVec.emplace_back();

    // Main loop.
    application.run();

    // Cleanup.
    Renderer::shutdown();
    return 0;
}
