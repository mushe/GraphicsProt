#include "ShapeDrawer.h"
#include "Utility/ModelGenerator.h"
#include "Core/Mesh.h"
#include <glm/gtc/constants.hpp>

namespace ShapeDrawer
{
    struct InstancingBuffer : InstancingUniformBufferBase
    {
		Vec3 position = Vec3(0);
        int shapeType = 0;
        Vec3 scale = Vec3(0);
        float directionTheta = 0;
        Vec4 color = Vec4(1);

        float ascii = 0;
        float dummy1 = 0;
        float dummy2 = 0;
        float dummy3 = 0;
	};

    shared_ptr<Material> mat_;
    shared_ptr<Mesh> mesh_;

    const int instanceCount_ = 1000000;
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

    auto tex = Texture::FromPNG("../Textures/font_inconsolata.png");
    
    mat_ = Material::Create(
        "Shape2D.vert", "Shape2D.frag",
        0,
        sizeof(InstancingBuffer) * instanceCount_,
        {tex}
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

void ShapeDrawer::Rect(Vec2 position, Vec2 scale, Vec4 color)
{
    if(IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = Vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = Vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 0;
    instancingBuffer_[currentFrameDrawCount_].color = color;

    currentFrameDrawCount_++;
}

void ShapeDrawer::Circle(Vec2 position, Vec2 scale, Vec4 color)
{
    if (IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = Vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = Vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 1;
    instancingBuffer_[currentFrameDrawCount_].color = color;

    currentFrameDrawCount_++;
}

void ShapeDrawer::Line(Vec2 start, Vec2 end, float width, Vec4 color)
{
	if (IsLimit(currentFrameDrawCount_)) return;

	instancingBuffer_[currentFrameDrawCount_].position = Vec3(start.x, start.y, 0);
	instancingBuffer_[currentFrameDrawCount_].scale = Vec3(end.x, end.y, width * 0.001);
	instancingBuffer_[currentFrameDrawCount_].shapeType = 2;
    instancingBuffer_[currentFrameDrawCount_].color = color;

	currentFrameDrawCount_++;
}

void ShapeDrawer::Triangle(Vec2 position, Vec2 direction, Vec2 scale, Vec4 color)
{
    if (IsLimit(currentFrameDrawCount_)) return;

    instancingBuffer_[currentFrameDrawCount_].position = Vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = Vec3(scale.x, scale.y, 0);
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


void Text(std::string str, Vec2 position, float scale, Vec4 color)
{
    ShapeDrawer::Text(str, position, scale, color);
}

void ShapeDrawer::Text(std::string str, Vec2 position, float scale, Vec4 color)
{
    scale = scale * 0.03f;
    float xScale = scale * 0.5f;
    for(int i = 0; i < str.size(); i++)
        Char(str[i], position + Vec2(xScale * i, 0.0), Vec2(xScale, scale), color);    
}

void ShapeDrawer::Char(char c, Vec2 position, Vec2 scale, Vec4 color)
{
    if(IsLimit(currentFrameDrawCount_)) return;

    int ascii = static_cast<int>(c);
    ascii = clamp(ascii, 0, 127);

    std::string s(1, c);

    instancingBuffer_[currentFrameDrawCount_].position = Vec3(position.x, position.y, 0);
    instancingBuffer_[currentFrameDrawCount_].scale = Vec3(scale.x, scale.y, 0);
    instancingBuffer_[currentFrameDrawCount_].shapeType = 4;
    instancingBuffer_[currentFrameDrawCount_].color = color;
    instancingBuffer_[currentFrameDrawCount_].ascii = (float)ascii;

    currentFrameDrawCount_++;
}