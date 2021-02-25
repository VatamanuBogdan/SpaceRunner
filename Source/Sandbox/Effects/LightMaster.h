#pragma once
#include <Engine.h>

#include <cstdint>
#include <set>

class Light {
public:
    explicit Light(const glm::vec3 &color) noexcept
    : Color(color) {
    }

    glm::vec3 Color;
};

class AmbientLight : public Light {
public:
    AmbientLight(const glm::vec3 &color, float strength) noexcept
    : Light(color), Strength(strength) {
    }

    float Strength;
};

class SimpleLight : public Light {
public:
    SimpleLight(const glm::vec3 &color, const glm::vec3 &position) noexcept
    : Light(color), Position(position) {
    }

    glm::vec3 Position;
};


class SpotLight : public SimpleLight {
public:
    SpotLight(const glm::vec3 &color, const glm::vec3 &position, const glm::vec3 &direction, float cutOff)
    : SimpleLight(color, position), direction(direction), cutOff(cutOff) {
    }

    glm::vec3 direction;
    float cutOff;
};


class LightMaster {
public:
    LightMaster() = delete;

    static void Init();

    static void AddNewLight(SimpleLight *light);
    static void AddNewLight(SpotLight *light);
    static void RemoveLight(SimpleLight *light);
    static void RemoveLight(SpotLight *light);

    static void SetAmbient(const AmbientLight &light);
    static void ApplyLights(const glm::vec3 &cameraPosition);

private:
    static void ApplyAmbientLight(Shader *shader, const glm::vec3 &cameraPosition);
    static void ApplyPointLights(Shader* shader);
    static void ApplySpotLights(Shader* shader);

private:
    static constexpr uint32_t MaxSimpleLights = 15;
    static constexpr uint32_t MaxSpotLights = 15;
    static std::set<SimpleLight*> s_PointLights;
    static std::set<SpotLight*> s_SpotLights;
    static AmbientLight s_AmbientLight;
};

