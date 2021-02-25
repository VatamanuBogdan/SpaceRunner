#pragma once
#include <Core/Engine.h>
#include <Component/SimpleScene.h>

#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Entities/Platform.h"
#include "Sandbox/Entities/Player.h"
#include "Sandbox/Entities/Obstacle.h"
#include "Sandbox/Entities/Bonus.hpp"

#include "Sandbox/Utility/Skybox.h"
#include "Sandbox/Utility/Camera.h"
#include "Sandbox/Utility/Paths.h"

#include "Sandbox/Preprocessing/Blur.h"
#include "Sandbox/Mechanics/Collisions.h"
#include "Sandbox/UI/StatusHud.h"


#include "Sandbox/Managers/Platforms.h"
#include "Sandbox/Managers/GPUManagers.h"
#include "Sandbox/Managers/Decorations.h"
#include "Sandbox/Managers/Bonuses.h"


class SpaceRunner : public SimpleScene {
public:
    SpaceRunner() = default;

    void Init() override;

private:
    void InitTextures();

    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;
    void OnKeyPress(int key, int mods) override;
    void OnInputUpdate(float deltaTime, int mods) override;

private:
    glm::vec2 m_Resolution;
    Camera* m_CurrentCamera;
    FirstPersonCamera* m_FirstPersonCamera;
    ThirdPersonCamera* m_ThirdPersonCamera;
    Skybox* m_Skybox;
    StatusHud* m_Hud;
    Blur* m_Blur;
    float m_SkyRotation = 3.14f, m_SkyRotationSpeed = 0.05f;
    Player* m_Player;
    Platforms* m_Platforms;
};
