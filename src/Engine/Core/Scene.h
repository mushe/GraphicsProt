#pragma once
#include "Core/Engine.h"

class Scene
{
public:
    Scene()
    {
        Debug::Log("Scene()");
    }
    virtual ~Scene(){Debug::Log("~Scene()");}

    virtual void Start(shared_ptr<Engine> engine) = 0;
    virtual bool Update(shared_ptr<Engine> engine) = 0; // return true if closing scene
    virtual void Release() = 0;
    void Close(){ Release(); }
};
