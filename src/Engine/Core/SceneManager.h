#pragma once
#include "Core/Engine.h"
#include "Core/Scene.h"

class SceneManager
{
public:
    SceneManager(shared_ptr<Engine> engine) : engine_(engine), currentScene_(nullptr)
    {
        Debug::Log("SceneManager()");
    }
    virtual ~SceneManager(){Debug::Log("~Scene()");}

    bool UpdateCurrentScene();
    void OpenScene(shared_ptr<Scene> scene);

    void Init(shared_ptr<Engine> engine);

private:
    void CloseCurrentScene();
    shared_ptr<Engine> engine_;

private:
    shared_ptr<Scene> currentScene_;
};
