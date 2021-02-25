#include "SpaceRunner.h"

#include "Sandbox/Effects/LightMaster.h"

Bonus *bonus;
Decorations *decoManager;

void SpaceRunner::Init() {
    MeshesManager::Init();
    ShadersManager::Init();
    TexturesManager::Init();
    PlatformTypes::Init();
    LightMaster::Init();
    Bonuses::Init();

    InitTextures();
    m_Resolution = window->GetResolution();
    m_FirstPersonCamera = new FirstPersonCamera(
            glm::radians(70.0f), m_Resolution.x / m_Resolution.y, 0.1f, 200.0f,
            glm::vec3(0.0f), glm::vec3(0.0f, glm::radians(180.0f), 0.0f)
            );
    m_ThirdPersonCamera = new ThirdPersonCamera(
            glm::radians(70.0f), m_Resolution.x / m_Resolution.y, 0.1f, 200.0f,
                glm::vec3(0.0f), glm::vec3(glm::radians(-20.0f), glm::radians(180.0f), 0), 5.0f
            );
    m_CurrentCamera = m_ThirdPersonCamera;

    m_Platforms = new Platforms();
    m_Player = new Player(
            Transform(glm::vec3(0, 2.0f, 0), glm::vec3(0), glm::vec3(0.5)), m_Platforms
            );
    m_CurrentCamera->Follow(m_Player->GetTransform().position);
    m_Hud = new StatusHud();
    m_Blur = new Blur(m_Resolution.x, m_Resolution.y);

    decoManager = new Decorations();
    bonus = new Bonus(Transform(glm::vec3(0, 2.0f, 5.0f)));
}

void SpaceRunner::InitTextures() {
    const char *textures[6] = {"right.png", "left.png", "top.png", "bottom.png", "front.png", "back.png"};
    m_Skybox = new Skybox((Resources::TexturePath + "SpaceSkybox/").c_str(), textures);
}

void SpaceRunner::FrameStart() {
    m_Resolution = window->GetResolution();
}

static float SleepTime = 0;
static float Score = 0.0f;

void SpaceRunner::Update(float deltaTimeSeconds) {
    SleepTime += deltaTimeSeconds;

    if (SleepTime < 3.5f) {
        return;
    }

    bonus->Update(deltaTimeSeconds, window);

    Score += Bonuses::CheckCollision(*m_Player);
    m_Player->CollisionsUpdate();
    m_Platforms->Update(deltaTimeSeconds, window);
    m_Player->Update(deltaTimeSeconds, window);
    m_CurrentCamera->Follow(m_Player->GetTransform().position);
    m_SkyRotation += m_SkyRotationSpeed * deltaTimeSeconds;
    m_Hud->Update(deltaTimeSeconds, window);
    m_Hud->SetSpeedPercent(m_Player->GetVelocityPercent());
    m_Hud->SetFuelPercent(m_Player->GetFuelPercent());
    m_Hud->SetLifeNum(m_Player->GetLife());
    m_Hud->SetScore(Score);

    LightMaster::ApplyLights(m_CurrentCamera->GetPosition());

    Bonuses::Update(deltaTimeSeconds, window);
    decoManager->SetVelocity(glm::vec3(0, 0, -m_Player->GetVelocity().z));
    decoManager->Update(deltaTimeSeconds, window);
}

void SpaceRunner::FrameEnd() {
    glViewport(0, 0, m_Resolution.x, m_Resolution.y);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_Player->IsDestroyed() || SleepTime < 3.5f) {
        m_Blur->Bind();
        m_Blur->Clear(0, 0, 0);
    }

    m_CurrentCamera->ComputeProjection();   m_CurrentCamera->ComputeView();
    const glm::mat4& proj = m_CurrentCamera->GetProjection();
    const glm::mat4& view = m_CurrentCamera->GetView();
    m_Skybox->Render(proj, glm::rotate(glm::mat4(1), 0.1f * (float) Engine::GetElapsedTime(), glm::vec3(0, 1, 1)));
    decoManager->Render(proj, view);
    Bonuses::Render(proj, view);


    m_Platforms->Render(proj, view);
    m_Player->Render(proj, view);

    if (m_Player->IsDestroyed() || SleepTime < 3.5f) {
        m_Blur->UnBind();
        m_Blur->Render();
    }
    else {
        m_Hud->Render();
    }
}

void SpaceRunner::OnKeyPress(int key, int mods) {

}

void SpaceRunner::OnInputUpdate(float deltaTime, int mods) {
    static bool pressed = false;
    if (window->KeyHold(GLFW_KEY_C)) {
        if (!pressed) {
            m_CurrentCamera = m_CurrentCamera == m_FirstPersonCamera ? m_ThirdPersonCamera : m_FirstPersonCamera;
            m_Player->ToggleFirstThird();
            m_Hud->ToggleFirstThird();
        }
        pressed = true;
    }
    else {
        pressed = false;
    }
}
