#pragma once
#include "Sandbox/Entities/Entity.h"

class Satellite : public Entity {
public:
    explicit Satellite(const Transform& transform, float timeAcc = 0.0f)
    : Entity(transform), m_Body(LoadBody()), m_RightPanel(LoadRightPanel()), m_LeftPanel(LoadLeftPanel()),
    m_Parabolic(LoadParabolic()), m_Support(LoadSupport()), m_Emitter(LoadEmitter()), m_Back(LoadBack()) {
        m_TimeAcc = timeAcc;
    }

    void Update(float deltaTime, WindowObject* window) override {
        m_TimeAcc += deltaTime;

        m_Transform.rotations.y = 0.55f * glm::sin(m_TimeAcc);
        m_Transform.ComputeModel();
        const glm::mat4 &model = m_Transform.GetModelMatrix();

        glm::mat4 panelRotation = glm::rotate(glm::mat4(1.0f),
                                              glm::sin(m_TimeAcc) / 2,
                                              glm::vec3(1, 0, 0)
                                              );

        m_Body.GetTransform().ComputeModel(model);
        m_RightPanel.GetTransform().ComputeModel(model * panelRotation);
        m_LeftPanel.GetTransform().ComputeModel(model * panelRotation);
        m_Parabolic.GetTransform().ComputeModel(model);
        m_Support.GetTransform().ComputeModel(model);
        m_Emitter.GetTransform().ComputeModel(model);
        m_Back.GetTransform().ComputeModel(model);
    }

    void Render(const glm::mat4& proj, const glm::mat4& view) override {
        m_Body.Render(proj, view);
        m_RightPanel.Render(proj, view);
        m_LeftPanel.Render(proj, view);
        m_Parabolic.Render(proj, view);
        m_Support.Render(proj, view);
        m_Emitter.Render(proj, view);
        m_Back.Render(proj, view);
    }

private:
    static TexturedEntity LoadBody() {
        Mesh* mesh = MeshesManager::Get("Cylinder");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("GrayMetal");
        Transform transform(
                glm::vec3(0, 0.006828, -0.003884),
                glm::vec3(0.944223, 0, 0),
                glm::vec3(0.548475, 0.859844, 0.548475)
                );
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity LoadRightPanel() {
        Mesh* mesh = MeshesManager::Get("Cube");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("SolarPanel");
        Transform transform(
                glm::vec3(-1.5, 0, 0),
                glm::vec3(-0.859874853, 0, 0),
                glm::vec3(1.76934, 0.03264, 1.018374)
        );
        return {mesh, shader, texture, transform};
    }


    static TexturedEntity LoadLeftPanel() {
        Mesh* mesh = MeshesManager::Get("Cube");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("SolarPanel");
        Transform transform(
                glm::vec3(1.5, 0, 0),
                glm::vec3(-0.859874853, 0, 0),
                glm::vec3(1.76934, 0.03264, 1.018374)
        );
        return {mesh, shader, texture, transform};
    }



    static TexturedEntity LoadParabolic() {
        Mesh* mesh = MeshesManager::Get("Hemisphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("GrayMetal");
        Transform transform(
                glm::vec3(0, -0.785815, -1.05299),
                glm::vec3(0.944223, 0, 0),
                glm::vec3(0.549082, 0.391487, 0.549082)
        );
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity LoadSupport() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("RedMetal");
        Transform transform(
                glm::vec3(0, -0.486636, -0.654078),
                glm::vec3(0.944223, 0, 0),
                glm::vec3(0.287063,0.287063, 0.287063)
        );
        return {mesh, shader, texture, transform};
    }

    static TexturedEntity LoadEmitter() {
        Mesh* mesh = MeshesManager::Get("Cone");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("GrayMetal");
        Transform transform(
                glm::vec3(0, -0.768443, -1.02983),
                glm::vec3(0.944223, 0, 3.1415926535),
                glm::vec3(0.150629,0.282736, 0.150629)
        );
        return {mesh, shader, texture, transform};
    }


    static TexturedEntity LoadBack() {
        Mesh* mesh = MeshesManager::Get("Sphere");
        Shader* shader = ShadersManager::Get("Textured");
        Texture2D* texture = TexturesManager::Get("Metallic");
        Transform transform(
                glm::vec3(0, 0.484189 , 0.640375),
                glm::vec3(0.944223, 0, 0),
                glm::vec3(0.289941, 0.289941, 0.289941)
        );
        return {mesh, shader, texture, transform};
    }

private:
    TexturedEntity m_Body;
    TexturedEntity m_RightPanel;
    TexturedEntity m_LeftPanel;
    TexturedEntity m_Parabolic;
    TexturedEntity m_Support;
    TexturedEntity m_Emitter;
    TexturedEntity m_Back;
    float m_TimeAcc;
};