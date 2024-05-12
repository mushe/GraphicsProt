#pragma once
#include "Core/Engine.h"
#include "Core/Scene.h"

class SceneManager
{
public:
    SceneManager(shared_ptr<Engine> engine) : engine_(engine), currentScene_(nullptr)
    {
    }
    virtual ~SceneManager(){ }

    bool UpdateCurrentScene();
    void OpenScene(shared_ptr<Scene> scene);
    void CloseCurrentScene();

private:
    shared_ptr<Engine> engine_;

private:
    shared_ptr<Scene> currentScene_;
};
