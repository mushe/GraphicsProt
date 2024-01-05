#include "ShapeDrawer.h"
#include "Utility/ModelGenerator.h"
#include "Core/Mesh.h"
#include <glm/gtc/constants.hpp>

namespace ShapeDrawer
{
    struct InstancingBuffer : InstancingUniformBufferBase
    {
		glm::vec3 position = glm::vec3(0);
        int shapeType = 0;
        glm::vec3 scale = glm::vec3(0);
        float directionTheta = 0;
        glm::vec4 color = glm::vec4(1);
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

void ShapeDrawer::Rect(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
{
    if(IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = glm::vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = glm::vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 0;
    instancingBuffer_[currentFrameDrawCount_].color = color;

    currentFrameDrawCount_++;
}

void ShapeDrawer::Circle(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
{
    if (IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = glm::vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = glm::vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 1;
    instancingBuffer_[currentFrameDrawCount_].color = color;

    currentFrameDrawCount_++;
}

void ShapeDrawer::Line(glm::vec2 start, glm::vec2 end, float width, glm::vec4 color)
{
	if (IsLimit(currentFrameDrawCount_)) return;

	instancingBuffer_[currentFrameDrawCount_].position = glm::vec3(start.x, start.y, 0);
	instancingBuffer_[currentFrameDrawCount_].scale = glm::vec3(end.x, end.y, width * 0.001);
	instancingBuffer_[currentFrameDrawCount_].shapeType = 2;
    instancingBuffer_[currentFrameDrawCount_].color = color;

	currentFrameDrawCount_++;
}

void ShapeDrawer::Triangle(glm::vec2 position, glm::vec2 direction, glm::vec2 scale, glm::vec4 color)
{
    if (IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = glm::vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = glm::vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 3;
    instancingBuffer_[currentFrameDrawCount_].color = color;
    instancingBuffer_[currentFrameDrawCount_].directionTheta = glm::atan(direction.y, direction.x) - glm::half_pi<float>();

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
