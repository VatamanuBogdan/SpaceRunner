#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Managers/GPUManagers.h"
#include "Sandbox/Effects/LightMaster.h"

#include <array>

class LightSource : public Entity {
public:
    explicit LightSource(const Transform &transform, float timeAcc = 0.0f)
    : Entity(transform), m_Sphere(LoadLight()), m_Pillar(LoadPillar()),
        m_PillarsTransforms(LoadPillarsTransforms()),
        m_Light(glm::vec3(0.5f, 0.5f, 0.5f), transform.position){
        m_TimeAcc = timeAcc;
    }

    void Update(float deltaTime, WindowObject *window) override {
        m_TimeAcc += deltaTime;
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(glm::sin(0.3 * m_TimeAcc) / 4 + 1));
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), 0.1f * m_TimeAcc, glm::vec3(1, 1, 0));
        const glm::mat4 &model = m_Transform.ComputeModel();
        m_Sphere.GetTransform().ComputeModel(model * scale);
        for (auto& transf : m_PillarsTransforms) {
            transf.ComputeModel(model * rotation);
        }
        m_Light.Position = m_Transform.position;
    }

    void Render(const glm::mat4 &proj, const glm::mat4 &view) override {
        m_Sphere.Render(proj, view);
        for (auto& transf : m_PillarsTransforms) {
            m_Pillar.GetTransform() = transf;
            m_Pillar.Render(proj, view);
        }
    }

private:
    static TexturedEntity LoadPillar() {
        Mesh* mesh = MeshesManager::Get("Cube");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("Green");
        Transform transform(glm::vec3(0));
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity LoadLight() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("BlueLiquid");
        Transform transform(glm::vec3(0), glm::vec3(0), glm::vec3(0.65));
        return {mesh, shader, texture, transform};
    }

    static std::array<Transform, 12> LoadPillarsTransforms() {
        return {
            Transform(glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec3(0.04f, 0.04f, 2)),
            Transform(glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec3(0.04f, 0.04f, 2)),
            Transform(glm::vec3(1, -1, 0), glm::vec3(0), glm::vec3(0.04f, 0.04f, 2)),
            Transform(glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0.04f, 0.04f, 2)),
            Transform(glm::vec3(-1, 0, -1), glm::vec3(0), glm::vec3(0.04f, 2, 0.04f)),
            Transform(glm::vec3(-1, 0, 1), glm::vec3(0), glm::vec3(0.04f, 2, 0.04f)),
            Transform(glm::vec3(1, 0, -1), glm::vec3(0), glm::vec3(0.04f, 2, 0.04f)),
            Transform(glm::vec3(1, 0, 1), glm::vec3(0), glm::vec3(0.04f, 2, 0.04f)),
            Transform(glm::vec3(0, -1, -1), glm::vec3(0), glm::vec3(2, 0.04f, 0.04f)),
            Transform(glm::vec3(0, -1, 1), glm::vec3(0), glm::vec3(2, 0.04f, 0.04f)),
            Transform(glm::vec3(0, 1, -1), glm::vec3(0), glm::vec3(2, 0.04f, 0.04f)),
            Transform(glm::vec3(0, 1, 1), glm::vec3(0), glm::vec3(2, 0.04f, 0.04f)),
        };
    }

private:
    TexturedEntity m_Sphere;
    TexturedEntity m_Pillar;
    std::array<Transform, 12> m_PillarsTransforms;
    SimpleLight m_Light;
    float m_TimeAcc;
};