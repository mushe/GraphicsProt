#pragma once
#include "Core/Scene.h"


class Minimum : public Scene
{
public:
    Minimum(){ }
    virtual ~Minimum(){  }

    void Start(shared_ptr<Engine> engine) override;
    bool Update(shared_ptr<Engine> engine) override;
    void Release() override;

private:
    shared_ptr<Mesh> mesh_;
    shared_ptr<Texture> tex_;
    shared_ptr<Material> mat_;
    Camera camera_;
};