#include "ShapeDrawer.h"
#include "Utility/ModelGenerator.h"
#include "Core/Mesh.h"

namespace ShapeDrawer
{
    struct InstancingBuffer : InstancingUniformBufferBase
    {
		glm::vec2 position = glm::vec2(0);
		glm::vec2 scale = glm::vec2(0);
	};

    shared_ptr<Material> mat_;
    shared_ptr<Mesh> mesh_;

    const int instanceCount_ = 5000;
    Camera camera_;

    std::vector<InstancingBuffer> instancingBuffer_(instanceCount_);

    int currentFrameDrawCount_ = 0;
};

void ShapeDrawer::Init()
{
    auto quad = ModelGenerator::GenerateDisplayQuad();
    mesh_ = Mesh::Create();
    mesh_->SetIndices(quad.indices);
    mesh_->SetVertices(quad.vertices);
    mesh_->Init();
    
    mat_ = Material::Create(
        "Shape2D.vert", "Shape2D.frag",
        0,
        sizeof(InstancingBuffer) * instanceCount_,
        {}
    );

    mat_->SetInstancingUniformBuffer(instancingBuffer_.data());
    mesh_->SetMaterial(mat_);

}

void ShapeDrawer::OnBeginRenderToScreen()
{
    currentFrameDrawCount_ = 0;
}

void ShapeDrawer::OnEndRenderToScreen()
{
    if(currentFrameDrawCount_ == 0) return;

    mesh_->Draw(camera_, currentFrameDrawCount_);
}

void ShapeDrawer::Rect(glm::vec2 position, glm::vec2 scale)
{
    if(IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = position;
    instancingBuffer_[currentFrameDrawCount_].scale = scale;

    currentFrameDrawCount_++;
}

bool ShapeDrawer::IsLimit(int count)
{
    if (currentFrameDrawCount_ + 1 > instanceCount_)
    {
        Debug::Log("ShapeDrawer exceed limit :  max = " + std::to_string(instanceCount_) + " , called = " + std::to_string(currentFrameDrawCount_ + 1) + " times");
        return true;
    }
    return false;
}
