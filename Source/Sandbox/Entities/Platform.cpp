#include "Sandbox/Entities/Platform.h"
#include "Sandbox/Managers/GPUManagers.h"
#include "Sandbox/Managers/Bonuses.h"


PlatformType PlatformTypes::Blue;
PlatformType PlatformTypes::Red;
PlatformType PlatformTypes::Yellow;
PlatformType PlatformTypes::Orange;
PlatformType PlatformTypes::Green;
PlatformType PlatformTypes::Purple;
PlatformType PlatformTypes::Lime;

void PlatformTypes::Init() {
    PlatformTypes::Blue = TexturesManager::Get("BluePlatform");
    PlatformTypes::Red = TexturesManager::Get("RedPlatform");
    PlatformTypes::Yellow = TexturesManager::Get("YellowPlatform");
    PlatformTypes::Orange = TexturesManager::Get("OrangePlatform");
    PlatformTypes::Green = TexturesManager::Get("GreenPlatform");
    PlatformTypes::Purple = TexturesManager::Get("PurplePlatform");
    PlatformTypes::Lime = TexturesManager::Get("LimePlatform");
}

Randomizer Platform::s_Random;

Platform::Platform(const glm::vec3 &position, float width, float length, PlatformType color, const glm::vec3 &velocity)
: Entity(Transform(position, glm::vec3(0.0f, glm::pi<float>(), 0.0f), glm::vec3(width, 0.2f, length))),
  m_Mesh(MeshesManager::Get("Cube")), m_Shader(ShadersManager::Get("Textured")), m_Color(color),
  m_Velocity(velocity), m_UpperPlane(glm::vec3(0.0f), width, length) {
    m_UpperPlane.SetPosition(glm::vec3(position.x, position.y + m_Transform.scales.y / 2, position.z));
    m_Transform.ComputeModel();

    if (color != PlatformTypes::Red && color != PlatformTypes::Lime) {
        float rand = s_Random();
        if (rand < 0.1f) {
            constexpr float size = 1.25f;
            glm::vec3 obstaclePos = glm::vec3(position.x, position.y + size / 2,
                                              position.z + (s_Random() - 0.5f) * length / 2);
            m_Obstacle = new Obstacle(
                    Transform(obstaclePos, glm::vec3(0.0f), glm::vec3(width, size, size)),
                    m_Mesh, m_Shader, m_Color, m_Velocity
            );
        } else if (rand < 0.5f) {
            Bonuses::AddBonus(Bonus(Transform(
                    glm::vec3(position.x, position.y + 2.0f, position.z + (s_Random() - 0.5f) * length / 2))));
        }
    }
}

Platform::~Platform() {
    delete m_Obstacle;
}

void Platform::Update(float deltaTime, WindowObject *window) {
    if (m_Obstacle != nullptr) m_Obstacle->Update(deltaTime, window);
    m_Transform.position += deltaTime * m_Velocity;
    m_UpperPlane.SetPosition(glm::vec3(
            m_Transform.position.x,
            m_Transform.position.y + m_Transform.scales.y / 2,
            m_Transform.position.z )
    );
    m_Transform.ComputeModel();
}

void Platform::Render(const glm::mat4 &projection, const glm::mat4 &view) {
    m_Shader->Use();
    glUniformMatrix4fv(m_Shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(m_Shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(m_Shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_Transform.GetModelMatrix()));
    if (m_Color) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Color->GetTextureID());
        glUniform1i(m_Shader->GetUniformLocation("texture_1"), 0);
    }

    glBindVertexArray(m_Mesh->GetBuffers()->VAO);
    glDrawElements(m_Mesh->GetDrawMode(), static_cast<int>(m_Mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
    if (m_Obstacle != nullptr) m_Obstacle->Render(projection, view);
}
