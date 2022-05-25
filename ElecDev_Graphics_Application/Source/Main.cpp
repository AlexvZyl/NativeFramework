#include "Utilities/Platform/Preprocessor.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"

// Testing.
#include "Utilities/Memory/FreeList.h"
#include "Utilities/Logger/Logger.h"

class TestClass
{
public:

    TestClass() 
    {
        std::cout << "TestClass::Constructor()\n";
    }

    ~TestClass() 
    {
        std::cout << "TestClass::Destructor()\n";
    }

    inline void myFunction() 
    {
        std::cout << "TestClass::myFunction()\n";
    }

    // Data.
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
};

void push(FreeList<TestClass>& fl) 
{
    int index = fl.push({});
    std::cout << "Pushed Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void emplace(FreeList<TestClass>& fl)
{
    int index = fl.emplace(TestClass());
    fl[index].myFunction();
    std::cout << "Emplaced Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

void pop(FreeList<TestClass>& fl, int index)
{
    fl.pop(index);
    fl[index].myFunction();
    std::cout << "Popped Index: " << std::to_string(index) << ", ";
    std::cout << "Capacity: " << std::to_string(fl.capacity()) << ", ";
    std::cout << "Size: " << std::to_string(fl.size()) << ".\n";
}

int main(int, char**)
{
    // TESTING!
    FreeList<TestClass> freeList(0, 2);
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);

    // Iteration.
    for (auto& entry : freeList)
    {
        entry.myFunction();
    }

    // Initialisation.
    Application application;
    Renderer::initialise();
    
    // Main loop.
    application.run();

    // Cleanup.
    Renderer::shutdown();
    return 0;
}
