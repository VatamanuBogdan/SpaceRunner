#pragma once
#include "Entity.h"
#include "Sandbox/Managers/Platforms.h"
#include "Sandbox/Mechanics/Collisions.h"
#include "Sandbox/Entities/Decorations/SpaceShip.h"
#include "Sandbox/Effects/LightMaster.h"

#include <Core/Engine.h>

class Player : public SpaceShip {
public:
    enum State : uint32_t {
        Normal = 0, HighSpeed = 1, Invulnerable = 2, Destroying = 4, Destroyed = 8
    };

public:
    Player(const Transform& transform, Platforms* platforms);

    void CollisionsUpdate();

    void Update(float deltaTime, WindowObject* window) override;
    void Render(const glm::mat4& projection, const glm::mat4& view) override;

    float GetVelocityPercent() const { return m_State & Player::State::HighSpeed ? 1.0f : m_Velocity.z / s_MaxForwardVelocity; }
    float GetFuelPercent() const { return m_Fuel / m_MaxFuel; }
    bool IsDestroyed() const { return m_State & Player::State::Destroyed; }
    uint16_t GetLife() const { return m_Life; }
    const glm::vec3& GetVelocity() const { return m_Velocity; }
    const Hitbox::Sphere& GetSphere() const { return m_Sphere; }


    void ToggleFirstThird() { m_ThirdPerson = !m_ThirdPerson; }

private:
    void ApplyBuff(PlatformType platformColor);
    void InputUpdate(float deltaTime, WindowObject* window);


private:
    Hitbox::CirclePlane m_DownPlane;
    Hitbox::Sphere m_Sphere;
    SpotLight m_Light;
    bool m_ThirdPerson = true;
    float m_GravityAcceleration = 20.0f;
    glm::vec3 m_Velocity = glm::vec3(0.0f, 0.0f, 4.0f);
    float m_Fuel = 100.0f, m_MaxFuel = 100.0f;
    float m_InvulnerableTime = 0.0f, m_HighSpeedTime = 0.0f;
    uint16_t m_Life = 4;
    uint32_t m_State = State::Normal;
    Platforms* m_Platforms;

private:
    static const float s_MinForwardVelocity;
    static const float s_MaxForwardVelocity;
};

