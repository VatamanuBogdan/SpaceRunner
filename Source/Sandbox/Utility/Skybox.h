#pragma once
#include <Core/Engine.h>
#include <string>

class Skybox {
public:
    Skybox(const char* path, const char* textures[6]);

    ~Skybox();

    void Render(const glm::mat4& projection, const glm::mat4& view);

private:
    bool LoadTextures(const char* path, const char* textures[6]);

private:
    static void InitCubeData();
    static void FreeCubeData();

private:
    uint32_t m_TextureID = 0;
    Shader* m_Shader = nullptr;

private:
    static uint32_t s_VAO, s_VBO;
    static uint32_t s_CounterRef;
};
