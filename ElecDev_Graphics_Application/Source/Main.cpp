//==============================================================================================================================================//
//  Includes.																																	//
//==============================================================================================================================================//

#include "Utilities/Platform/Preprocessor.h"
#include "Application/Application.h"
#include "OpenGL/Renderer/RendererGL.h"

// Testing.
#include "Utilities/Memory/FreeList.h"
class Test 
{
public:
    Test() 
    {
    
    }
    ~Test() 
    {
    
    }
    Test(const Test& other) 
    {
        
    }

    unsigned index = 0;
    unsigned data2 = 0;
    unsigned data3 = 0;
};
inline void loop(FreeList<Test>& fl) 
{
    std::cout << "Capacity: " << fl.capacity() << ".\n.";
    std::cout << "Allocated: " << fl.allocated() << ".\n.";
    std::cout << "Count: " << fl.count() << ".\n";
    std::cout << "Size: " << fl.size() << ".\n\n";
    for (auto& entry : fl)
        std::cout << "Index: " << entry.index << "\n.";
}
inline void emplace(FreeList<Test>& fl)
{
    int index = fl.emplace();
    fl[index].index = index;
}

//==============================================================================================================================================//
//  Entrypoint.																																	//
//==============================================================================================================================================//

int main(int, char**)
{
    // Testing FreeLists again...
    FreeList<Test> freeList;
    emplace(freeList);
    emplace(freeList);
    emplace(freeList);
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

//==============================================================================================================================================//
//  EOF.																																	    //
//==============================================================================================================================================//