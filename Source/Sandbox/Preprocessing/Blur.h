#pragma once
#include <Engine.h>

class Blur {
public:
    Blur(uint32_t width, uint32_t height);
    ~Blur();

    void Bind();
    void Clear(float r, float g, float b);
    void UnBind();
    void Render();

private:
    void InitQuad();

private:
    uint32_t m_FBO = 0;
    uint32_t m_RBO = 0;
    uint32_t m_ColorBuffer = 0;
    uint32_t m_QuadVAO = 0, m_QuadVBO = 0, m_QuadIBO;
    Shader* m_Shader;
};

