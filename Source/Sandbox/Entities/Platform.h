#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Mechanics/Collisions.h"
#include "Sandbox/Entities/Obstacle.h"
#include "Sandbox/Utility/Randomizer.hpp"

using PlatformType = Texture2D*;

class PlatformTypes {
public:
    PlatformTypes() = delete;

    static void Init();

    static PlatformType Blue;
    static PlatformType Red;
    static PlatformType Yellow;
    static PlatformType Orange;
    static PlatformType Green;
    static PlatformType Purple;
    static PlatformType Lime;
};

class Platform : public Entity {
public:
    Platform(const glm::vec3& position, float width, float length, PlatformType color, const glm::vec3& velocity);
    ~Platform();

public:
    void Update(float deltaTime, WindowObject* window) override;

    void Render(const glm::mat4& projection, const glm::mat4& view) override;

    const Hitbox::RectanglePlane& GetUpperPlane() const { return m_UpperPlane; }

    bool HasBuff() const { return m_Buff; }
    void ConsumeBuff() const {
        m_Buff = false;
        if (m_Color != PlatformTypes::Blue) {
            m_Color = PlatformTypes::Purple;
            if (m_Obstacle) {
                m_Obstacle->SetType(PlatformTypes::Purple);
            }
        }
    }
    PlatformType GetType() const { return m_Color; }

    bool HaveObstacle() const { return m_Obstacle != nullptr; }
    const Obstacle& GetObstacle() const { return *m_Obstacle; }

private:
    Mesh* m_Mesh;
    Shader* m_Shader;
    Obstacle* m_Obstacle = nullptr;
    Hitbox::RectanglePlane m_UpperPlane;
    mutable bool m_Buff = true;
    mutable PlatformType m_Color;
    const glm::vec3& m_Velocity;
    static Randomizer s_Random;
};