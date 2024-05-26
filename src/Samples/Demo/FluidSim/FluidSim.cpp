#include "FluidSim.h"

void FluidSim::Start(shared_ptr<Engine> engine)
{
    reserveSceneClose_ = false;
    reserveSceneCloseWaitFrame_ = 0;

    computeUBO_.resize(particleCount_);
    ResetSimulation();

    // fluid instancing mesh
    auto sphere = ModelGenerator::GenerateSphere(10);
    mesh_ = Mesh::Create();
    mesh_->SetIndices(sphere.indices);
    mesh_->SetVertices(sphere.vertices);
    mesh_->Init();
    
    mat_ = Material::Create(
        "FluidRender.vert", "FluidRender.frag",
        sizeof(FluidCustomUBO),
        sizeof(FluidRenderUBO) * particleCount_,
        {},
        sharingBuffer_,
        false
    );
    mat_->SetInstancingUniformBuffer(computeUBO_.data());
    mat_->SetUniformBufferData(&customUbo_);
    mesh_->SetMaterial(mat_);

    mesh_fluid_additive_ = Mesh::Create();
    mesh_fluid_additive_->SetIndices(sphere.indices);
    mesh_fluid_additive_->SetVertices(sphere.vertices);
    mesh_fluid_additive_->Init();
    mat_fluid_additive_ = Material::Create(
        "FluidRender.vert", "FluidRender.frag",
        sizeof(FluidCustomUBO),
        sizeof(FluidRenderUBO) * particleCount_,
        {},
        sharingBuffer_,
        true
    );
    mat_fluid_additive_->SetInstancingUniformBuffer(computeUBO_.data());
    mat_fluid_additive_->SetUniformBufferData(&customUbo_);
    mesh_fluid_additive_->SetMaterial(mat_fluid_additive_);


    // render targets
    rt_ = RenderTarget::Create(2048, 2048);
    rt_depth_processed_ = RenderTarget::Create(2048, 2048);
    rt_floor_ = RenderTarget::Create(2048, 2048);
    rt_fluid_additive_ = RenderTarget::Create(2048, 2048);


    // blur
    InitBlurForDepth();
    InitBlurForColor();


    // floor
    floorMesh_ = Mesh::Create();
    auto plane = ModelGenerator::GeneratePlane();
    floorMesh_->SetIndices(plane.indices);
    floorMesh_->SetVertices(plane.vertices);
    floorMesh_->Init();
    floorMesh_->SetScale(Vec3(6.0f));
    floorMesh_->SetPosition(Vec3(0.0f, -1.25f, 0.0f));
    floorTex_ = Texture::FromPNG("../Textures/grid.png");
    floorMat_ = Material::Create("Standard.vert", "UnlitTexture.frag", { floorTex_ });
    floorMesh_->SetMaterial(floorMat_);


    // composite
    postProcessMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_FluidComposite.frag",
        sizeof(FluidCompositeUBO),
        //{ rt_->GetDepthTexture() }
        { rt_1_1_up_->GetColorTexture() , rt_floor_->GetColorTexture(), rt_1_1_up_color_->GetColorTexture()}
    );
    postProcessMat_->SetUniformBufferData(&ubo_);


    // process depth
    depthProcessMat_ = Material::Create
    (
        "renderToScreen.vert", "PP_FluidDepthProcess.frag",
        sizeof(FluidDepthProcessUBO),
        { rt_->GetDepthTexture() }
    );
    depthProcessMat_->SetUniformBufferData(&ubo_);

    // camera settings
    OrbitalControl::SetOrbit(1.181012f, 6.184282f, 6.233325f);
    OrbitalControl::Update(camera_, 0.0f, 0.0f);

}


void FluidSim::InitBlurForDepth()
{
    rt_1_2_down_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_4_down_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_8_down_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_16_down_ = RenderTarget::Create(res_1_16_.x, res_1_16_.y);
    rt_1_8_up_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_4_up_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_2_up_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_1_up_ = RenderTarget::Create(fullRes_.x, fullRes_.y);

    mat_1_2_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        //{ rt_->GetDepthTexture() }
        { rt_depth_processed_->GetColorTexture() }
        //{ rt_fluid_additive_->GetColorTexture() }
    );
    ubo_1_2_down_.resolution = res_1_2_;
    mat_1_2_down_->SetUniformBufferData(&ubo_1_2_down_);

    mat_1_4_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_2_down_->GetColorTexture() }
    );
    ubo_1_4_down_.resolution = res_1_4_;
    mat_1_4_down_->SetUniformBufferData(&ubo_1_4_down_);

    mat_1_8_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_4_down_->GetColorTexture() }
    );
    ubo_1_8_down_.resolution = res_1_8_;
    mat_1_8_down_->SetUniformBufferData(&ubo_1_8_down_);


    mat_1_16_down_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_8_down_->GetColorTexture() }
    );
    ubo_1_16_down_.resolution = res_1_16_;
    mat_1_16_down_->SetUniformBufferData(&ubo_1_16_down_);


    mat_1_8_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_16_down_->GetColorTexture() }
    );
    ubo_1_8_up_.resolution = res_1_8_;
    mat_1_8_up_->SetUniformBufferData(&ubo_1_8_up_);


    mat_1_4_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_8_up_->GetColorTexture() }
    );
    ubo_1_4_up_.resolution = res_1_4_;
    mat_1_4_up_->SetUniformBufferData(&ubo_1_4_up_);


    mat_1_2_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_4_up_->GetColorTexture() }
    );
    ubo_1_2_up_.resolution = res_1_2_;
    mat_1_2_up_->SetUniformBufferData(&ubo_1_2_up_);


    mat_1_1_up_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_2_up_->GetColorTexture() }
    );
    ubo_1_1_up_.resolution = fullRes_;
    mat_1_1_up_->SetUniformBufferData(&ubo_1_1_up_);
}

void FluidSim::InitBlurForColor()
{
    rt_1_2_down_color_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_4_down_color_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_8_down_color_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_16_down_color_ = RenderTarget::Create(res_1_16_.x, res_1_16_.y);
    rt_1_8_up_color_ = RenderTarget::Create(res_1_8_.x, res_1_8_.y);
    rt_1_4_up_color_ = RenderTarget::Create(res_1_4_.x, res_1_4_.y);
    rt_1_2_up_color_ = RenderTarget::Create(res_1_2_.x, res_1_2_.y);
    rt_1_1_up_color_ = RenderTarget::Create(fullRes_.x, fullRes_.y);

    mat_1_2_down_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        //{ rt_->GetDepthTexture() }
        //{ rt_depth_processed_->GetColorTexture() }
        { rt_fluid_additive_->GetColorTexture() }
    );
    mat_1_2_down_color_->SetUniformBufferData(&ubo_1_2_down_);

    mat_1_4_down_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_2_down_color_->GetColorTexture() }
    );
    mat_1_4_down_color_->SetUniformBufferData(&ubo_1_4_down_);

    mat_1_8_down_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_4_down_color_->GetColorTexture() }
    );
    mat_1_8_down_color_->SetUniformBufferData(&ubo_1_8_down_);

    mat_1_16_down_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Down.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_8_down_color_->GetColorTexture() }
    );
    mat_1_16_down_color_->SetUniformBufferData(&ubo_1_16_down_);

    mat_1_8_up_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_16_down_color_->GetColorTexture() }
    );
    mat_1_8_up_color_->SetUniformBufferData(&ubo_1_8_up_);

    mat_1_4_up_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_8_up_color_->GetColorTexture() }
    );
    mat_1_4_up_color_->SetUniformBufferData(&ubo_1_4_up_);

    mat_1_2_up_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_4_up_color_->GetColorTexture() }
    );
    mat_1_2_up_color_->SetUniformBufferData(&ubo_1_2_up_);

    mat_1_1_up_color_ = Material::Create
    (
        "renderToScreen.vert", "PP_Blur_Up.frag",
        sizeof(FluidBlurPostProcessUBO),
        { rt_1_2_up_color_->GetColorTexture() }
    );
    mat_1_1_up_color_->SetUniformBufferData(&ubo_1_1_up_);
}

void FluidSim::ResetSimulation()
{
    for (auto& u : computeUBO_)
    {
        float theta = Random::Range(0.0f, 2.0f * PI);
        float phi = Random::Range(0.0f, PI);
        float r = Random::Range(0.0f, 1.9f);
        u.pos = glm::vec3
        (
            r * sin(phi) * cos(theta) + fieldScale_ / 2.0f,
            r * sin(phi) * sin(theta) + fieldScale_ / 2.0f,
            r * cos(phi) + fieldScale_ / 2.0f
        );

        float uy = Random::Range(-0.005f, 0.01f);
        u.vel = Vec3(0.0f, uy, 0.0f);
        u.density = 0.0f;
        u.pressure = 0.0f;
    }

    VkDeviceSize bufferSize = sizeof(FluidRenderUBO) * particleCount_;
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(VulkanCore::GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, computeUBO_.data(), (size_t)bufferSize);
    vkUnmapMemory(VulkanCore::GetDevice(), stagingBufferMemory);

    VulkanUtil::CreateBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingBuffer_, sharingBufferMemory_);
    VulkanUtil::CopyBuffer(stagingBuffer, sharingBuffer_, bufferSize);

    vkDestroyBuffer(VulkanCore::GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), stagingBufferMemory, nullptr);
    // ----------------------------------------------------------------

    // initialize compute shader
    computeShader_.Init(particleCount_, sharingBuffer_);

}


bool FluidSim::Update(shared_ptr<Engine> engine)
{
    if (Input::KeyDown(InputCode::Key::Escape)) engine->Quit();
    engine->BeginFrame();

    // wait for closing scene
    if(reserveSceneClose_)
    {
        reserveSceneCloseWaitFrame_++;
        if(reserveSceneCloseWaitFrame_ > 5)
        {
            engine->EndFrame();
            return true;
        }
    }

    //OrbitalControl::Update(camera_, 0.0f, 0.0f);
    //OrbitalControl::LogOrbit();

    // execute compute shader
    rep(i, 3) computeShader_.Execute();

    rt_->BeginRenderToTexture();
    mesh_->Draw(camera_, particleCount_);
    rt_->EndRenderToTexture();

    rt_fluid_additive_->BeginRenderToTexture();
    mesh_fluid_additive_->Draw(camera_, particleCount_);
    rt_fluid_additive_->EndRenderToTexture();

    rt_floor_->BeginRenderToTexture();
    floorMesh_->Draw(camera_);
    rt_floor_->EndRenderToTexture();


    // blur
    engine->BlitToRenderTarget(depthProcessMat_, rt_depth_processed_);
    engine->BlitToRenderTarget(mat_1_2_down_, rt_1_2_down_);
    engine->BlitToRenderTarget(mat_1_4_down_, rt_1_4_down_);
    engine->BlitToRenderTarget(mat_1_8_down_, rt_1_8_down_);
    engine->BlitToRenderTarget(mat_1_16_down_, rt_1_16_down_);
    engine->BlitToRenderTarget(mat_1_8_up_, rt_1_8_up_);
    engine->BlitToRenderTarget(mat_1_4_up_, rt_1_4_up_);
    engine->BlitToRenderTarget(mat_1_2_up_, rt_1_2_up_);
    engine->BlitToRenderTarget(mat_1_1_up_, rt_1_1_up_);

    engine->BlitToRenderTarget(mat_1_2_down_color_, rt_1_2_down_color_);
    engine->BlitToRenderTarget(mat_1_4_down_color_, rt_1_4_down_color_);
    engine->BlitToRenderTarget(mat_1_8_down_color_, rt_1_8_down_color_);
    engine->BlitToRenderTarget(mat_1_16_down_color_, rt_1_16_down_color_);
    engine->BlitToRenderTarget(mat_1_8_up_color_, rt_1_8_up_color_);
    engine->BlitToRenderTarget(mat_1_4_up_color_, rt_1_4_up_color_);
    engine->BlitToRenderTarget(mat_1_2_up_color_, rt_1_2_up_color_);
    engine->BlitToRenderTarget(mat_1_1_up_color_, rt_1_1_up_color_);


    engine->BeginRenderToScreen();
    engine->BlitToScreen(postProcessMat_);

    engine->OnGUI([&]()
    {
        if (GUI::Button(" return to menu ")) reserveSceneClose_ = true;
        GUI::Space();
        GUI::FloatSlider(blurScale_, "blur scale", 0.0f, 8.0f);

        GUI::Color(ubo_.ColorA, "Water Color A");
        GUI::Color(ubo_.ColorB, "Water Color B");
        // GUI::Float(ubo_.diffusePower, "Diffuse Power", 0.001f);
        // GUI::Float(ubo_.diffuseMultiply, "Diffuse Multiply", 0.001f);
        // GUI::Float(ubo_.specularPower, "Specular Power", 0.001f);
        // GUI::Float(ubo_.specularMultiply, "Specular Multiply", 0.001f);
        GUI::Float(ubo_.floorColorBlendPower, "Floor Color Blend Power", 0.001f);
        GUI::Float(ubo_.floorColorBlendMultiply, "Floor Color Blend Multiply", 0.001f);
    });

    ubo_1_2_down_.scale = blurScale_;
    ubo_1_4_down_.scale = blurScale_;
    ubo_1_8_down_.scale = blurScale_;
    ubo_1_16_down_.scale = blurScale_;
    ubo_1_8_up_.scale = blurScale_;
    ubo_1_4_up_.scale = blurScale_;
    ubo_1_2_up_.scale = blurScale_;
    ubo_1_1_up_.scale = blurScale_;

    engine->EndRenderToScreen();
    engine->EndFrame();
    return false;
}


void FluidSim::Release()
{
    computeShader_.Release();

    vkDestroyBuffer(VulkanCore::GetDevice(), sharingBuffer_, nullptr);
    vkFreeMemory(VulkanCore::GetDevice(), sharingBufferMemory_, nullptr);

    postProcessMat_ = nullptr;
    rt_ = nullptr;

    mesh_ = nullptr;
    mat_ = nullptr;

    rt_1_2_down_ = nullptr;
    rt_1_4_down_ = nullptr;
    rt_1_8_down_ = nullptr;
    rt_1_16_down_ = nullptr;
    rt_1_8_up_ = nullptr;
    rt_1_4_up_ = nullptr;
    rt_1_2_up_ = nullptr;
    rt_1_1_up_ = nullptr;

    mat_1_2_down_ = nullptr;
    mat_1_4_down_ = nullptr;
    mat_1_8_down_ = nullptr;
    mat_1_16_down_ = nullptr;
    mat_1_8_up_ = nullptr;
    mat_1_4_up_ = nullptr;
    mat_1_2_up_ = nullptr;
    mat_1_1_up_ = nullptr;

    rt_depth_processed_ = nullptr;
    depthProcessMat_ = nullptr;

    floorMesh_ = nullptr;
    floorTex_ = nullptr;
    floorMat_ = nullptr;
    rt_floor_ = nullptr;

    rt_fluid_additive_ = nullptr;
    mat_fluid_additive_ = nullptr;
    mesh_fluid_additive_ = nullptr;

    rt_1_2_down_color_ = nullptr;
    rt_1_4_down_color_ = nullptr;
    rt_1_8_down_color_ = nullptr;
    rt_1_16_down_color_ = nullptr;
    rt_1_8_up_color_ = nullptr;
    rt_1_4_up_color_ = nullptr;
    rt_1_2_up_color_ = nullptr;
    rt_1_1_up_color_ = nullptr;

    mat_1_2_down_color_ = nullptr;
    mat_1_4_down_color_ = nullptr;
    mat_1_8_down_color_ = nullptr;
    mat_1_16_down_color_ = nullptr;
    mat_1_8_up_color_ = nullptr;
    mat_1_4_up_color_ = nullptr;
    mat_1_2_up_color_ = nullptr;
    mat_1_1_up_color_ = nullptr;
}
