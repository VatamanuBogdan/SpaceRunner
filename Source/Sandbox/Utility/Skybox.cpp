#include "Sandbox/Utility/Skybox.h"
#include "Sandbox/Managers/GPUManagers.h"

#include <stb/stb_image.h>
#include <iostream>

uint32_t Skybox::s_VAO = 0;
uint32_t Skybox::s_VBO = 0;
uint32_t Skybox::s_CounterRef = 0;

Skybox::Skybox(const char *path, const char *textures[])
: m_Shader(ShadersManager::Get("Skybox")) {
    LoadTextures(path, textures);
    if (s_CounterRef == 0) {
        InitCubeData();
    }
    s_CounterRef++;
}

Skybox::~Skybox() {
    if (--s_CounterRef == 0) {
        FreeCubeData();
    }
    glDeleteTextures(1, &m_TextureID);
}

void Skybox::Render(const glm::mat4 &projection, const glm::mat4 &view) {
    glDepthMask(GL_FALSE);
    m_Shader->Use();
    glUniformMatrix4fv(m_Shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 View = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(m_Shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    glBindVertexArray(s_VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

bool Skybox::LoadTextures(const char *path, const char* textures[6]) {
    std::string texturePath;   texturePath.reserve(50);
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    int width, height, nrChannels;
    uint8_t* data;
    for(uint8_t i = 0; i < 6; i++) {
        texturePath = path ,   texturePath += textures[i];
        data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else {
            std::cerr << "[Error] :: Failed to load skybox texture1 " << texturePath << std::endl;
            return false;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return true;
}

void Skybox::InitCubeData() {
    float data[] = {
            -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &s_VAO);   glBindVertexArray(s_VAO);
    glGenBuffers(1, &s_VBO);    glBindBuffer(GL_VERTEX_ARRAY, s_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glBindVertexArray(0);
    glBindBuffer(GL_VERTEX_ARRAY, 0);
}

void Skybox::FreeCubeData() {
    glDeleteVertexArrays(1, &s_VAO);
    glDeleteBuffers(1, &s_VBO);
}
