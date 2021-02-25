#pragma once
#include "Sandbox/Managers/GPUManagers.h"

#include <Engine.h>

class StatusHud {
public:
    StatusHud()
    : m_Shader(ShadersManager::Get("Hud")) {
        constexpr float xPos = 0.3f, yPos = 0.175, offset = 0.02f;
        {
            std::vector<VertexFormat> vertices = {
                    VertexFormat(glm::vec3(-xPos - 2 * offset, 0, 0.0f)),
                    VertexFormat(glm::vec3(-xPos, 2 * offset, 0.0f)),
                    VertexFormat(glm::vec3(xPos, 2 * offset, 0.0f)),
                    VertexFormat(glm::vec3(-xPos + offset, 2 * yPos + 2 * offset, 0.0f)),
                    VertexFormat(glm::vec3(xPos + offset, 2 * yPos + 2 * offset, 0.0f))
            };
            {
                std::vector<uint16_t> indices = {0, 1, 3, 3, 1, 2, 3, 2, 4};
                m_HudMesh = new Mesh("Hud");
                m_HudMesh->InitFromData(vertices, indices);
            }
        }
        {
            std::vector<VertexFormat> vertices = {
                    VertexFormat(glm::vec3(0, 2 * offset + 0.055f, 0)),
                    VertexFormat(glm::vec3(0.5, 2 * offset + 0.055f, 0)),
                    VertexFormat(glm::vec3(0.5,  2 * offset + 0.1f, 0)),
                    VertexFormat(glm::vec3(0, 2 * offset + 0.1f, 0))
            };
            std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
            m_FuelMesh = new Mesh("Fuel");
            m_FuelMesh->InitFromData(vertices, indices);
        }
        {
            std::vector<VertexFormat> vertices = {
                    VertexFormat(glm::vec3(0, 2 * offset + 0.155f, 0)),
                    VertexFormat(glm::vec3(0.5, 2 * offset + 0.155f, 0)),
                    VertexFormat(glm::vec3(0.5,  2 * offset + 0.2f, 0)),
                    VertexFormat(glm::vec3(0, 2 * offset + 0.2f, 0))
            };
            std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};
            m_SpeedMesh = new Mesh("Speed");
            m_SpeedMesh->InitFromData(vertices, indices);
        }
        {
            std::vector<VertexFormat> vertices = {
                    VertexFormat(glm::vec3(0, 13 * offset + 0.08f, 0)),
                    VertexFormat(glm::vec3(0.04, 13 * offset + 0.08f, 0)),
                    VertexFormat(glm::vec3(0.02,13 * offset + 0.01f, 0)),
            };
            std::vector<uint16_t> indices = {0, 1, 2};
            m_LifeMesh = new Mesh("Life");
            m_LifeMesh->InitFromData(vertices, indices);
        }
        {
            m_ScoreBar = MeshesManager::Get("Quad");
        }
    }

    ~StatusHud() {
        delete m_LifeMesh;
        delete m_FuelMesh;
        delete m_HudMesh;
        delete m_LifeMesh;
    }

public:

    void Update(float deltaTime, WindowObject* window) {
        static bool keyPressed = false;
        m_TimeAccumulator += deltaTime;

        if (window->KeyHold(GLFW_KEY_TAB)) {
            if (!keyPressed) {
                keyPressed = true;
                m_Opened = !m_Opened;
            }
        }
        else {
            keyPressed = false;
        }

        if (m_Opened) {
            m_ScaleFactor = glm::min(m_ScaleFactor + 4 * deltaTime, 1.0f);
        }
        else {
            m_ScaleFactor = glm::max(m_ScaleFactor - 4 * deltaTime, 0.0f);
        }
    }

    void Render() {
        if (m_ScaleFactor != 0.0f) {
            m_Shader->Use();
            int xScaleLocation = m_Shader->GetUniformLocation("xScale");
            int yScaleLocation = m_Shader->GetUniformLocation("yScale");
            int colorLocation = m_Shader->GetUniformLocation("Color");
            int positionLocation = m_Shader->GetUniformLocation("Position");

            glUniform1f(yScaleLocation, m_ScaleFactor);
            glUniform2f(positionLocation, 0.15, m_YPos);
            glUniform1f(xScaleLocation, m_FuelPercent);
            glUniform4f(colorLocation, FuelColor.x, FuelColor.y, FuelColor.z, FuelColor.a);
            m_FuelMesh->Render();
            glUniform1f(xScaleLocation, m_SpeedPercent);
            float light = glm::abs(glm::cos(m_TimeAccumulator)) / 4 + 0.7f;
            glUniform4f(colorLocation, light * SpeedColor.x, light * SpeedColor.y, light * SpeedColor.z , SpeedColor.a);
            m_SpeedMesh->Render();

            glUniform1f(xScaleLocation, 1.0f);
            glUniform4f(colorLocation, Black.x, Black.y, Black.z, Black.a);
            m_FuelMesh->Render();
            m_SpeedMesh->Render();
            glUniform4f(colorLocation, SpeedColor.x, SpeedColor.y, SpeedColor.z, SpeedColor.a);
            constexpr float offset = 0.06f;
            for (uint16_t i = 0; i < m_LifeNum; i++) {
                glUniform2f(positionLocation, 0.155 + i * offset, m_YPos);
                m_LifeMesh->Render();
            }

            glUniform2f(positionLocation, 0.4, m_YPos);
            glUniform4f(colorLocation, HudColor.x, HudColor.y, HudColor.z, HudColor.a);
            m_HudMesh->Render();

            glUniform1f(xScaleLocation, 0.04f);
            glUniform1f(yScaleLocation, m_Score / m_MaxScore);
            glUniform2f(positionLocation, 1.0f, 0.0f);
            glUniform4f(colorLocation, 0.75f, 0.3f, 0.0f, 0.2f);
            m_ScoreBar->Render();

        }
    }

    void SetFuelPercent(float percent) { m_FuelPercent = glm::max(0.0f, glm::min(percent, 1.0f)); }
    void SetSpeedPercent(float percent) { m_SpeedPercent = glm::max(0.0f, glm::min(percent, 1.0f)); }
    void SetLifeNum(uint16_t life) { m_LifeNum = life; }
    void SetScore(float score) { m_Score = score; }

    void ToggleFirstThird() { m_YPos = m_YPos == 0.15f ? -1.0f : 0.15f; }

private:
    bool m_Opened = true;
    float m_TimeAccumulator = 0, m_ScaleFactor = 1, m_YPos = 0.15f;
    Mesh *m_HudMesh, *m_FuelMesh, *m_SpeedMesh, *m_LifeMesh;
    Mesh *m_ScoreBar;
    Shader *m_Shader;
    float m_Score, m_MaxScore = 100.0f;
    uint16_t m_LifeNum = 0;
    float m_SpeedPercent = 0, m_FuelPercent = 0;
    const glm::vec4 SpeedColor = glm::vec4(0.3f, 1, 0.4, 0.2f);
    const glm::vec4 FuelColor = glm::vec4(0, 0.5f, 0.5f, 0.2f);
    const glm::vec4 HudColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.1f);
    const glm::vec4 Black = glm::vec4(0, 0, 0, 0.1f);
};