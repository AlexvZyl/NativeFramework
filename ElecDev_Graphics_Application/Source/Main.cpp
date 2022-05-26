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
        //std::cout << "TestStruct::Destructor()\n";
    }
};

class TestClass
{
public:

    TestClass() 
    {
        //std::cout << "TestClass::Constructor()\n";
        myUPtrString = std::make_unique<std::string>("This is my UPtr String yay!");
        myStruct = std::make_unique<TestStruct>();
    }

    ~TestClass() 
    {
        //std::cout << "TestClass::Destructor()\n";
    }

    TestClass(const TestClass& other) 
    {
        //std::cout << "TestClass::CopyConstructor()\n";
        myUPtrString = std::make_unique<std::string>("This is my UPtr String yay!");
        myStruct = std::make_unique<TestStruct>();
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

void emplace(FreeList<TestClass>& fl, bool display = false)
{
    int index = fl.emplace();
    fl[index].indexInList = index;
    if (!display) return;
    std::cout << "Emplaced Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void pop(FreeList<TestClass>& fl, int index, bool display = false)
{
    fl.pop(index);
    if (!display) return;
    std::cout << "Popped Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void loop(FreeList<TestClass>& fl) 
{
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", Size: " << std::to_string(fl.size()) << ".\n";
    for (auto& entry : fl)
        std::cout << "Index: " << std::to_string(entry.indexInList) << ".\n";
    std::cout << "\n";
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
    pop(freeList, 1);
    pop(freeList, 3);
    pop(freeList, 4);
    pop(freeList, 0);
    loop(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
    loop(freeList);

    freeList.pop(0, 5);
    loop(freeList);

    // Initialisation.
    Application application;
    Renderer::initialise();
    
    // Main loop.
    application.run();

    // Cleanup.
    Renderer::shutdown();
    return 0;
}
