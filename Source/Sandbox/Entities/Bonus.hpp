#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Managers/GPUManagers.h"
#include "Sandbox/Mechanics/Collisions.h"


class Bonus : public TexturedEntity {
public:
    explicit Bonus(const Transform& transform)
    : TexturedEntity(MeshesManager::Get("Cube"),
                     ShadersManager::Get("Textured"),
                     TexturesManager::Get("Box"),
                     transform),
     m_Hitbox(transform.position, transform.scales.x, transform.scales.y, transform.scales.z) {
    }

    void Update(float deltaTime, WindowObject *window) override {
        m_TimeAcc += deltaTime;
        m_Transform.position += deltaTime * m_Velocity;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,  0.5f * glm::sin(m_TimeAcc), 0.0f));
        m_Hitbox.SetPosition(model * glm::vec4(m_Transform.position, 1.0f));
        model = glm::rotate(model, 2 * m_TimeAcc, glm::vec3(0, 1.0f, 0));
        if (m_Collected) {
            m_Transform.scales -= 4 * deltaTime;
        }

        m_Transform.ComputeModel(glm::mat4(1.0f), model);
    }

    void Collect() { m_Collected = true; }
    bool IsCollected() const { return m_Collected; }
    bool IsDestroyed() const { return m_Transform.scales.x < 0.0f; }
    void SetSpeed(const glm::vec3 velocity) { m_Velocity = velocity; }
    const Hitbox::AABB& GetHitbox() const { return m_Hitbox; }


private:
    Hitbox::AABB m_Hitbox;
    glm::vec3 m_Velocity = glm::vec3(0.0f);
    bool m_Collected = false;
    float m_TimeAcc = 0.0f;
};