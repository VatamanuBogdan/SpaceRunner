#pragma once
#include <Core/Engine.h>

class Transform {
public:
    explicit Transform(const glm::vec3 &position,
                       const glm::vec3 &rotations = glm::vec3(0.0f),
                       const glm::vec3 &scales = glm::vec3(1.0f))
    : position(position), rotations(rotations), scales(scales) {
        ComputeModel();
    }

    const glm::mat4& ComputeModel(const glm::mat4& leftModel = glm::mat4(1.0f),
                                  const glm::mat4& rightModel = glm::mat4(1.0f)) {
        m_Model = glm::translate(leftModel, position);
        m_Model = glm::rotate(m_Model, rotations.x, glm::vec3(1.0f, 0, 0));
        m_Model = glm::rotate(m_Model, rotations.y, glm::vec3(0, 1.0f, 0));
        m_Model = glm::rotate(m_Model, rotations.z, glm::vec3(0, 0, 1.0f));
        m_Model = glm::scale(m_Model, scales);
        m_Model *= rightModel;
        return m_Model;
    }

    const glm::mat4& GetModelMatrix() const { return m_Model; }

public:
    glm::vec3 position;
    glm::vec3 rotations;
    glm::vec3 scales;

private:
    glm::mat4 m_Model;
};


class Entity {
public:
    explicit Entity(const Transform& transform)
    : m_Transform(transform) {
    }

    virtual void Update(float deltaTime, WindowObject* window) = 0;

    virtual void Render(const glm::mat4& proj, const glm::mat4& view) = 0;

    Transform& GetTransform() { return m_Transform; }

    virtual ~Entity() = default;

protected:
    Transform m_Transform;
};


class TexturedEntity : public Entity {
public:
    TexturedEntity(Mesh* mesh, Shader* shader, Texture2D* texture,
                   const Transform& transform)
    : Entity(transform), m_Mesh(mesh), m_Shader(shader), m_Texture(texture) {
    }

    void Update(float deltaTime, WindowObject* window) override {}

    void Render(const glm::mat4& proj, const glm::mat4& view) override {
        glUseProgram(m_Shader->program);

        // Bind model matrix
        GLint loc_model_matrix = glGetUniformLocation(m_Shader->program, "Model");
        glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(m_Transform.GetModelMatrix()));

        // Bind view matrix
        glm::mat4 viewMatrix = view;
        int loc_view_matrix = glGetUniformLocation(m_Shader->program, "View");
        glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Bind projection matrix
        glm::mat4 projectionMatrix = proj;
        int loc_projection_matrix = glGetUniformLocation(m_Shader->program, "Projection");
        glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        if (m_Texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_Texture->GetTextureID());
            glUniform1i(m_Shader->GetUniformLocation("texture_1"), 0);
        }


        // Draw the object
        glBindVertexArray(m_Mesh->GetBuffers()->VAO);
        glDrawElements(m_Mesh->GetDrawMode(), static_cast<int>(m_Mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
    }

protected:
    Mesh* m_Mesh;
    Shader* m_Shader;
    Texture2D* m_Texture;
};