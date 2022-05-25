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

int main(int, char**)
{
    // TESTING!
    FreeList<TestClass> freeList(0, 2);
    TestClass myClass;
    freeList.push(myClass);
    freeList.push(myClass);
    freeList.push(myClass);
    freeList.pop(2);

    // Initialisation.
    Application application;
    Renderer::initialise();

    // Main loop.
    application.run();

    // Cleanup.
    Renderer::shutdown();
    return 0;
}
