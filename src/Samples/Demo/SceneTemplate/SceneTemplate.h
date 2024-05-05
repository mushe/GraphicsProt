#pragma once
#include "Core/Scene.h"


class SceneTemplate : public Scene
{
public:
    SceneTemplate()
    {
    }
    virtual ~SceneTemplate() { }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
};