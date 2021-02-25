#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Managers/GPUManagers.h"

#include <array>

class SpaceShip : public Entity {
public:
    explicit SpaceShip(const Transform& transform, float timeAcc = 0.0f)
    : Entity(transform), m_Body(LoadBody()), m_GlassCabin(LoadGlassCabin()), m_Engine(LoadEngine()),
      m_Support(LoadSupport()), m_EnginesTransf(LoadEnginesTransf()), m_SupportsTransf(LoadSupportsTransf()) {
        m_Transform.scales = glm::vec3(1.35);
        m_TimeAcc = timeAcc;
    }

public:
    void Update(float deltaTime, WindowObject *window) override {
        m_TimeAcc += deltaTime;
        m_Transform.rotations.x = 0.2f * glm::sin(m_TimeAcc);
        m_Transform.rotations.z = 0.1f * glm::sin(m_TimeAcc);
        m_Transform.ComputeModel();
        const glm::mat4 model = m_Transform.GetModelMatrix();
        glm::mat4 vibration = glm::scale(glm::mat4(1), glm::vec3(1 - glm::sin(40 * m_TimeAcc) / 20));
        m_Body.GetTransform().ComputeModel(model);
        m_GlassCabin.GetTransform().ComputeModel(model);
        for (auto& transf : m_EnginesTransf)
            transf.ComputeModel(model, vibration);
        for (auto& transf : m_SupportsTransf)
            transf.ComputeModel(model);
    }

    void Render(const glm::mat4 &proj, const glm::mat4 &view) override {
        m_Body.Render(proj, view);
        m_GlassCabin.Render(proj, view);
        for (auto& transf : m_EnginesTransf) {
            m_Engine.GetTransform() = transf;
            m_Engine.Render(proj, view);
        }
        for (auto& transf : m_SupportsTransf) {
            m_Support.GetTransform() = transf;
            m_Support.Render(proj, view);
        }
    }

private:
    static TexturedEntity LoadBody() {
        return {MeshesManager::Get("Sphere"),
                ShadersManager::Get("Textured"),
                TexturesManager::Get("GraySteel"),
                Transform(
                    glm::vec3(-0.01 , 0, 0.06),
                    glm::vec3(0),
                    glm::vec3(0.347, 0.265, 1.168)
                )
        };
    }

    static TexturedEntity LoadGlassCabin() {
        return {MeshesManager::Get("Sphere"),
                ShadersManager::Get("Textured"),
                TexturesManager::Get("BlueGlass"),
                Transform(
                        glm::vec3(-0.013798  , 0.249674, -0.224059),
                        glm::vec3(0),
                        glm::vec3(0.146854, 0.096156, 0.427969)
                )
        };
    }

    static TexturedEntity LoadEngine() {
        return {MeshesManager::Get("Cylinder"),
                ShadersManager::Get("Textured"),
                TexturesManager::Get("Rust"),
                Transform(glm::vec3(0))
        };
    }

    static TexturedEntity LoadSupport() {
        return {MeshesManager::Get("Cube"),
                ShadersManager::Get("Textured"),
                TexturesManager::Get("GraySteel"),
                Transform(glm::vec3(0))
        };
    }

    static std::array<Transform, 2> LoadEnginesTransf() {
        return {
            Transform(glm::vec3(-0.887867 , 0, 0.764045), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(0.25, 0.35, 0.25)),
            Transform(glm::vec3(0.887867 , 0, 0.764045), glm::vec3(glm::radians(90.0f), 0, 0), glm::vec3(0.25, 0.35, 0.25))
        };
    }

    static std::array<Transform, 4> LoadSupportsTransf() {
        return {
            Transform(glm::vec3(-0.013798 , 0, 0.71888), glm::vec3(0), glm::vec3(2, 0.088, 0.454)),
            Transform(glm::vec3(-0.013798 , 0.68888, 0.71888), glm::vec3(0), glm::vec3(2, 0.07, 0.454)),
            Transform(glm::vec3(0.259349 , 0.355803, 0.727231), glm::vec3(0, 0, glm::radians(-43.2)), glm::vec3(0.1, 0.826, 0.1)),
            Transform(glm::vec3(-0.259349 , 0.355803, 0.727231), glm::vec3(0, 0, glm::radians(43.2)), glm::vec3(0.1, 0.826, 0.1)),
        };
    }

private:
    TexturedEntity m_Body;
    TexturedEntity m_GlassCabin;
    TexturedEntity m_Engine;
    TexturedEntity m_Support;
    std::array<Transform, 2> m_EnginesTransf;
    std::array<Transform, 4> m_SupportsTransf;
    float m_TimeAcc;
};