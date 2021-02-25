#pragma once
#include "Sandbox/Entities/Platform.h"
#include "Sandbox/Utility/Randomizer.hpp"

#include <Core/Engine.h>
#include <deque>

class Platforms {
public:
    Platforms()
    : m_Random() {
        SetGenerator();
    }

    void Update(float deltaTime, WindowObject* window);

    void Render(const glm::mat4& projection, const glm::mat4& view);

    const std::deque<Platform>& GetPlatforms() const { return m_Platforms; }

    void SetSpeed(float speed) { m_PlatformsVelocity.z = -speed; }

private:
    void SpawnNewRow(float zSpawnCoord);
    void SpawnPoint();
    void GarbagePoint();
    void SetGenerator();

private:
    Randomizer m_Random;
    std::deque<Platform> m_Platforms;
    glm::vec3 m_PlatformsVelocity = glm::vec3(0, 0, -4.0f);
    short m_LastColNum = 0;

public:
    class Constants {
    public:
        Constants() = delete;

        static const short ColNum;
        static const float MinWidth, MaxWidth;
        static const float MinLength, MaxLength;
        static const float MinHeight, MaxHeight;
        static const float SpaceLength;
        static const float MaxOffset;
    };
};

