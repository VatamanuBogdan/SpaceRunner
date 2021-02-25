#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Effects/LightMaster.h"
#include "Sandbox/Managers/GPUManagers.h"

class Ufo : public Entity {
public:
    explicit Ufo(const Transform& transform, float timeAcc = 0.0f)
    : Entity(transform), m_Lateral(LoadLateral()), m_Sphere(LoadSphere()),
        m_Ray(LoadRay()), m_Light(InitLight(transform.position)) {
        LightMaster::AddNewLight(&m_Light);
        m_TimeAcc = timeAcc;
    }

    Ufo(const Ufo& ufo)
    : Entity(ufo.m_Transform), m_Lateral(ufo.m_Lateral), m_Sphere(ufo.m_Sphere), m_Ray(ufo.m_Ray), m_Light(ufo.m_Light) {
        LightMaster::AddNewLight(&m_Light);
    }



    ~Ufo() {
        LightMaster::RemoveLight(&m_Light);
    }

    void Update(float deltaTime, WindowObject* window) override {
        m_TimeAcc += deltaTime;
        glm::vec4 lightDir = glm::vec4(0, -1, 0, 0);
        glm::mat4 model = glm::rotate(glm::mat4(1), 2.0f * m_TimeAcc, glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0, 0, 1));
        lightDir = -model * lightDir;
        model = glm::rotate(model, 0.4f * m_TimeAcc, glm::vec3(0, 1, 0));
        model = m_Transform.ComputeModel() * model;
        m_Sphere.GetTransform().ComputeModel(model);
        m_Lateral.GetTransform().ComputeModel(model);
        m_Ray.GetTransform().ComputeModel(model);
        m_Light.Position = m_Transform.position;
        m_Light.direction = lightDir;
    }

    void Render(const glm::mat4& proj, const glm::mat4& view) override {
        m_Sphere.Render(proj, view);
        m_Lateral.Render(proj, view);
        m_Ray.Render(proj, view);
    }

private:
    static TexturedEntity LoadLateral() {
        return {
            MeshesManager::Get("Sphere"),
            ShadersManager::Get("Textured"),
            TexturesManager::Get("Metallic"),
            Transform(
                    glm::vec3(0, -0.1f, 0),
                    glm::vec3(0, 0, 0),
                    glm::vec3(1, 0.2, 1)
                    )
        };
    }

    static TexturedEntity LoadSphere() {
        return {
            MeshesManager::Get("Sphere"),
            ShadersManager::Get("Textured"),
            TexturesManager::Get("Green"),
            Transform(
                    glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, 0),
                    glm::vec3(0.4, 0.4, 0.4)
                    )
        };
    }

    static TexturedEntity LoadRay() {
        return {
            MeshesManager::Get("Cone"),
            ShadersManager::Get("Textured"),
            TexturesManager::Get("Purple"),
            Transform(
                    glm::vec3(0, -0.7113, 0),
                    glm::vec3(0, 0, 0),
                    glm::vec3(0.75, 1.0, 0.75)
                    )
        };
    }

    static SpotLight InitLight(const glm::vec3 &position) {
        return {
            glm::vec3(0, 1.5, 1.5f),
            position,
            glm::vec3(0, 1, 0),
            glm::cos(glm::radians(35.0f))
        };
    }

private:
    TexturedEntity m_Lateral;
    TexturedEntity m_Sphere;
    TexturedEntity m_Ray;
    float m_TimeAcc = 0.0f;
    SpotLight m_Light;
};