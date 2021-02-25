#pragma once
#include <Core/Engine.h>

class Camera {
protected:
    Camera(float fov, float aspect, float near, float far, const glm::vec3& position)
    : m_Position(position), m_Forward(glm::vec3(0, 0, -1.0f)), m_Up(glm::vec3(0, 1.0f, 0)),
      m_Right(glm::vec3(1.0f, 0 ,0)), m_Fov(fov), m_Aspect(aspect), m_Near(near), m_Far(far) {
    }

    virtual ~Camera() = default;

public:
    void ComputeView() { m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up); }
    void ComputeProjection() { m_Projection = glm::perspective(m_Fov, m_Aspect, m_Near, m_Far); }

    const glm::mat4& GetProjection() const { return m_Projection; }
    const glm::mat4& GetView() const { return m_View; }

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetFov(float fov) { m_Fov = fov; }

    const glm::vec3& GetPosition() const { return m_Position; }
    float GetFov() const { return m_Fov; }

    void TranslateForward(float distance) { m_Position += m_Forward * distance; }
    void TranslateUpward(float distance) { m_Position += m_Up * distance; }
    void TranslateRight(float distance) { m_Position += m_Right * distance; }

public:
    virtual void RotateOX(float angle) = 0;
    virtual void RotateOY(float angle) = 0;
    virtual void RotateOZ(float angle) = 0;

    virtual void Follow(const glm::vec3& pos) = 0;

protected:
    glm::vec3 m_Position, m_Forward, m_Up, m_Right;
    float m_Fov, m_Aspect, m_Near, m_Far;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
};


class FirstPersonCamera : public Camera {
public:
    FirstPersonCamera(float fov, float aspect, float near, float far, const glm::vec3& position)
    : Camera(fov, aspect, near, far, position) {
    }

    FirstPersonCamera(float fov, float aspect, float near, float far,
                      const glm::vec3& position, const glm::vec3& rotation)
    : Camera(fov, aspect, near, far, position) {
        FirstPersonCamera::RotateOX(rotation.x);
        FirstPersonCamera::RotateOY(rotation.y);
        FirstPersonCamera::RotateOZ(rotation.z);
    }

    ~FirstPersonCamera() override = default;

public:
    void RotateOX(float angle) override {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, m_Right);
        m_Forward = glm::normalize(glm::vec3(rot * glm::vec4(m_Forward, 1.0f)));
        m_Up = glm::normalize(glm::vec3(rot * glm::vec4(m_Up, 1.0f)));
    }

    void RotateOY(float angle) override {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        m_Forward = glm::normalize(glm::vec3(rot * glm::vec4(m_Forward, 1)));
        m_Right = glm::normalize(glm::vec3(rot * glm::vec4(m_Right, 1)));
        m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
    }

    void RotateOZ(float angle) override {
        glm::mat4 rot = glm::rotate(glm::mat4(1), angle, m_Forward);
        m_Right = glm::vec3(rot * glm::vec4(m_Right, 1));
        m_Up = glm::vec3(rot * glm::vec4(m_Up, 1));
    }

    void Follow(const glm::vec3& pos) override { m_Position = pos; }
};


class ThirdPersonCamera : public FirstPersonCamera {
public:
    ThirdPersonCamera(float fov, float aspect, float near, float far,
                      const glm::vec3& position, float distance)
            : FirstPersonCamera(fov, aspect, near, far, position), m_Distance(distance) {
    }

    ThirdPersonCamera(float fov, float aspect, float near, float far,
                      const glm::vec3& position, const glm::vec3& rotation, float distance)
    : FirstPersonCamera(fov, aspect, near, far, position), m_Distance(distance) {
        ThirdPersonCamera::RotateOX(rotation.x);
        ThirdPersonCamera::RotateOY(rotation.y);
        ThirdPersonCamera::RotateOZ(rotation.z);
    }

    void Follow(const glm::vec3& pos) final { m_Position = pos - m_Distance * m_Forward; }

public:
    void RotateOX(float angle) final {
        m_Position += m_Forward * m_Distance;
        FirstPersonCamera::RotateOX(angle);
        m_Position -= m_Forward * m_Distance;
    }

    void RotateOY(float angle) final {
        m_Position += m_Forward * m_Distance;
        FirstPersonCamera::RotateOY(angle);
        m_Position -= m_Forward * m_Distance;
    }

    void RotateOZ(float angle) final {
        m_Position += m_Forward * m_Distance;
        FirstPersonCamera::RotateOZ(angle);
        m_Position -= m_Forward * m_Distance;
    }

protected:
    float m_Distance;
};