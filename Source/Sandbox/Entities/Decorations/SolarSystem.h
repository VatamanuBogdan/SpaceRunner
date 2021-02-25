#pragma once
#include "Sandbox/Entities/Entity.h"
#include "Sandbox/Managers/GPUManagers.h"


class SolarSystem : public Entity {
public:
    explicit SolarSystem(const Transform& transform)
    : Entity(transform), m_Earth(InitEarth()), m_Moon(InitMoon()),
        m_Venus(InitVenus()) , m_Sun(InitSun()), m_Light(glm::vec3(1.5f, 1.5f, 1.5f), transform.position)
    {
        LightMaster::AddNewLight(&m_Light);
    }

    void Update(float deltaTime, WindowObject* window) override {
        constexpr float earthSpeed = 0.2f;
        constexpr float venusSpeed = 0.3f;

        Transform& sunTransf = m_Sun.GetTransform();
        sunTransf.rotations.y += deltaTime;

        static float venusSunAngle = 0.0f;
        venusSunAngle += deltaTime * venusSpeed;
        glm::mat4 venusModel = glm::rotate(glm::mat4(1), venusSunAngle, glm::vec3(0, 1, 0));
        venusModel = glm::translate(venusModel, glm::vec3(6.0f, 0, 0));

        static float earthSunAngle = 0.0f;
        earthSunAngle += deltaTime * earthSpeed;
        glm::mat4 earthModel = glm::rotate(glm::mat4(1), earthSunAngle, glm::vec3(0, 1, 0));
        earthModel = glm::translate(earthModel, glm::vec3(8.0f, 0, 0));

        Transform& earthTransf = m_Earth.GetTransform();
        earthTransf.rotations.y += deltaTime;

        static float earthMoonAngle = 0.0f;
        earthMoonAngle += deltaTime;
        glm::mat4 moonModel= glm::rotate(glm::mat4(1), earthMoonAngle, glm::vec3(0, 1.0f, 0));
        moonModel = glm::translate(moonModel, glm::vec3(2, 0, 0));

        glm::mat4 model = m_Transform.ComputeModel();
        m_Earth.GetTransform().ComputeModel(model * earthModel);
        m_Moon.GetTransform().ComputeModel(model * earthModel * moonModel);
        m_Venus.GetTransform().ComputeModel(model * venusModel);
        m_Sun.GetTransform().ComputeModel(model);

        m_Light.Position = m_Transform.position;
    }

    void Render(const glm::mat4& proj, const glm::mat4& view) override {
        m_Earth.Render(proj, view);
        m_Moon.Render(proj, view);
        m_Venus.Render(proj, view);
        m_Sun.Render(proj, view);
    }

private:
    static TexturedEntity InitEarth() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader =  ShadersManager::Get("Textured");
        Texture2D* texture =  TexturesManager::Get("Earth");
        Transform transform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity InitMoon() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("Moon");
        Transform transform(glm::vec3(0), glm::vec3(0), glm::vec3(0.5f));
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity InitVenus() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("Venus");
        Transform transform(glm::vec3(0), glm::vec3(0), glm::vec3(0.75f));
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity InitSun() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Light");
        Texture2D* texture = TexturesManager::Get("Sun");
        Transform transform(glm::vec3(0), glm::vec3(0), glm::vec3(2.5f));
        return {mesh, shader, texture, transform};
    }

private:
    TexturedEntity m_Earth;
    TexturedEntity m_Moon;
    TexturedEntity m_Venus;
    TexturedEntity m_Sun;
    SimpleLight m_Light;
};