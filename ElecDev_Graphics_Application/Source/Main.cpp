#include "Utilities/Platform/Preprocessor.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"
#include "Utilities/Memory/FreeList.h"

struct TestStruct 
{
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
};

int main(int, char**)
{
    // TESTING!
    FreeList<TestStruct> freeList;
    freeList.allocate(10);
    auto [size, next, prev] = freeList.getSlotData(0);
    freeList.allocate(20);
    auto [size2, next2, prev2] = freeList.getSlotData(0);
    
    // Initialisation.
    Application application;
    Renderer::initialise();

    // Main loop.
    application.run();

    // Cleanup.
    Renderer::shutdown();
    return 0;
}