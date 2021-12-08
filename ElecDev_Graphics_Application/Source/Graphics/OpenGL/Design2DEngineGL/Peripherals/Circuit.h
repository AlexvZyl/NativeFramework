#pragma once
#include "Graphics/OpenGL/CoreGL/Entities/ManagedEntity.h"
#include "Cable.h"
#include "Component2D.h"
class Circuit :
    public ManagedEntity
{    
public:
    std::vector<std::shared_ptr<Component2D>> m_components;
    std::vector<std::shared_ptr<Cable>> m_cables;
    Circuit();
};

