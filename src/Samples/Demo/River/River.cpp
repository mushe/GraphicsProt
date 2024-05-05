#include "River.h"



void River::Start(shared_ptr<Engine> engine)
{
    mesh_ = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane(100);
    mesh_->SetIndices(plane.indices);
    mesh_->SetVertices(plane.vertices);
    mesh_->Init();
    mesh_->SetRotation(Vec3(0, 0, 0));
    mesh_->SetScale(Vec3(10.0f));
    tex1_ = Texture::FromPNG("../Textures/RiverGround.png");
    tex2_ = Texture::FromPNG("../Textures/RiverMask.png");
    mat_ = Material::Create
    (
        "Standard.vert", "river.frag",
        sizeof(RiverUBO),
        { tex1_, tex2_ }
    );
    mat_->SetUniformBufferData(&ubo_);
    mesh_->SetMaterial(mat_);


    meshRotation_ = Vec3(0, 0, 0);
    OrbitalControl::SetOrbit(0.95f, 7.0f, 17.0f);
    OrbitalControl::Update(camera_);
}


bool River::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();
    OrbitalControl::Update(camera_, 0.0f, 0.0f);
    bool closeScene = false;

    engine->BeginRenderToScreen();
    mesh_->Draw(camera_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) closeScene = true;
        GUI::Space();

        GUI::Float(distortionSpeed_, "distortionSpeed", 0.01f);
        GUI::Float(distortionScale_, "distortionScale", 0.01f);
        GUI::Float(distortionPosScale_, "distortionPosScale", 0.01f);
        GUI::Float(noiseScaleX_1_, "noiseScaleX_1", 0.01f);
        GUI::Float(noiseScaleY_1_, "noiseScaleY_1", 0.01f);
        GUI::Float(noiseSpeed_1_, "noiseSpeed_1", 0.01f);
        GUI::Float(noiseScaleX_2_, "noiseScaleX_2", 0.01f);
        GUI::Float(noiseScaleY_2_, "noiseScaleY_2", 0.01f);
        GUI::Float(noiseSpeed_2_, "noiseSpeed_2", 0.01f);
        GUI::Float(noiseThreshold_, "noiseThreshold", 0.01f);
        GUI::Float(noisePower_, "noisePower", 0.01f);
        GUI::Float(noiseMultiply_, "noiseMultiply", 0.01f);
        GUI::FloatSlider(colorLerp_, "colorLerp", 0.0f, 1.0f);
        GUI::FloatSlider(riverColR_, "riverColR", 0.0f, 1.0f);
        GUI::FloatSlider(riverColG_, "riverColG", 0.0f, 1.0f);
        GUI::FloatSlider(riverColB_, "riverColB", 0.0f, 1.0f);
        ubo_.distortionSpeed = distortionSpeed_;
        ubo_.distortionScale = distortionScale_;
        ubo_.distortionPosScale = distortionPosScale_;
        ubo_.noiseScaleX_1 = noiseScaleX_1_;
        ubo_.noiseScaleY_1 = noiseScaleY_1_;
        ubo_.noiseSpeed_1 = noiseSpeed_1_;
        ubo_.noiseScaleX_2 = noiseScaleX_2_;
        ubo_.noiseScaleY_2 = noiseScaleY_2_;
        ubo_.noiseSpeed_2 = noiseSpeed_2_;
        ubo_.noiseThreshold = noiseThreshold_;
        ubo_.noisePower = noisePower_;
        ubo_.noiseMultiply = noiseMultiply_;
        ubo_.colorLerp = colorLerp_;
        ubo_.riverColR = riverColR_;
        ubo_.riverColG = riverColG_;
        ubo_.riverColB = riverColB_;
    });

    engine->EndRenderToScreen();
    engine->EndFrame();
    return closeScene;
}


void River::Release()
{
}
