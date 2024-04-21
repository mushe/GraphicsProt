#include "SceneManager.h"

void SceneManager::OpenScene(shared_ptr<Scene> scene)
{
  if(!scene) return;
  
  CloseCurrentScene();

  scene->Start(engine_);
  currentScene_ = scene;
}

void SceneManager::CloseCurrentScene()
{
  if(currentScene_ != nullptr)
  {
    currentScene_->Release();
    currentScene_ = nullptr;
  }

  GUI::ChangeStyleToDefault();
}

bool SceneManager::UpdateCurrentScene()
{
  if(currentScene_ != nullptr)
  {
    bool close = currentScene_->Update(engine_);
    if(close)
    {
      CloseCurrentScene();
      return false;
    }
    else return true;
  }

  return false;
}