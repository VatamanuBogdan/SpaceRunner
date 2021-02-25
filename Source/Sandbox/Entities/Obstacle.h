#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Mechanics/Collisions.h"

#include <Engine.h>

class Obstacle : public Entity {
public:
    Obstacle(const Transform& transform, Mesh* mesh, Shader* shader,  Texture2D* color, const glm::vec3& velocity)
    : Entity(transform), m_Type(color), m_Velocity(velocity), m_Mesh(mesh), m_Shader(shader),
      m_AABB(transform.position, transform.scales.x, transform.scales.y, transform.scales.z) {
        m_Transform.ComputeModel();
    }

    void Update(float deltaTime, WindowObject* window) override;
    void Render(const glm::mat4& projection, const glm::mat4& view) override;

    const Hitbox::AABB& GetAABB() const { return m_AABB; }
    void SetType(Texture2D *type) { m_Type = type; }

private:
    Texture2D* m_Type;
    const glm::vec3& m_Velocity;
    Mesh* m_Mesh;
    Shader* m_Shader;
    Hitbox::AABB m_AABB;
    float m_TimeAcc = 0.0f;
};

