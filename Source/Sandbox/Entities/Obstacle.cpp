#include "Obstacle.h"


void Obstacle::Update(float deltaTime, WindowObject *window) {
    m_TimeAcc += deltaTime;
    float flipFactor = glm::max(0.1f, glm::abs(glm::sin(m_TimeAcc)));
    m_Transform.position += deltaTime * m_Velocity;
    m_AABB.SetYEdge(flipFactor * m_Transform.scales.y);
    m_AABB.SetPosition(
            m_Transform.position -
            glm::vec3(0, (1 - flipFactor) * m_Transform.scales.y, 0)
            );
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, -0.5f, 0));
    model = glm::scale(model, glm::vec3(1.0f, flipFactor, 1.0f));
    model =  glm::translate(model,glm::vec3(0, 0.5f, 0));
    m_Transform.ComputeModel(glm::mat4(1.0f), model);
}

void Obstacle::Render(const glm::mat4 &projection, const glm::mat4 &view) {
    m_Shader->Use();
    glUniformMatrix4fv(m_Shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(m_Shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(m_Shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_Transform.GetModelMatrix()));
    glBindVertexArray(m_Mesh->GetBuffers()->VAO);
    if (m_Type) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Type->GetTextureID());
        glUniform1i(m_Shader->GetUniformLocation("texture_1"), 0);
    }
    glDrawElements(m_Mesh->GetDrawMode(), static_cast<int>(m_Mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}
