#include "Sandbox/Managers/Platforms.h"
#include "Sandbox/Managers/GPUManagers.h"

const short Platforms::Constants::ColNum = 3;
const float Platforms::Constants::MinWidth = 1.5f;
const float Platforms::Constants::MaxWidth = 3.5f;
const float Platforms::Constants::MinLength = 10.0f;
const float Platforms::Constants::MaxLength = 25.0f;
const float Platforms::Constants::MinHeight = -0.5f;
const float Platforms::Constants::MaxHeight = 1.0f;
const float Platforms::Constants::SpaceLength = 200.0f;
const float Platforms::Constants::MaxOffset = 1.5f;


void Platforms::Update(float deltaTime, WindowObject *window) {
    for (auto& platform : m_Platforms) platform.Update(deltaTime, window);
    SpawnPoint();
    GarbagePoint();
}

void Platforms::Render(const glm::mat4 &projection, const glm::mat4 &view) {
    for (auto& platform : m_Platforms) platform.Render(projection, view);
}


void Platforms::SpawnNewRow(float zSpawnCoord) {
    static auto genColumn = [&]() {
        if (m_LastColNum == 0) return std::min(m_LastColNum + 1, Constants::ColNum - 1);
        if (m_LastColNum == Constants::ColNum) return std::max(0, m_LastColNum - 1);
        return m_Random() <= 0.5f ? m_LastColNum - 1 : m_LastColNum + 1;
    };
    static auto genPos = [&](short column, float lastZ, float length) {
        return glm::vec3(
                (column - Constants::ColNum / 2) * Constants::MaxWidth,
                m_Random() * (Constants::MaxHeight - Constants::MinHeight) + Constants::MinHeight,
                lastZ + length / 2 + m_Random() * Constants::MaxOffset
                );
    };
    static auto genColor = [&]() {
        float choice = m_Random();
        if (choice <= 0.7f) return PlatformTypes::Blue;
        if (choice <= 0.75f) return PlatformTypes::Yellow;
        if (choice <= 0.85f) return PlatformTypes::Green;
        if (choice <= 0.9f) return PlatformTypes::Orange;
        if (choice <= 0.95f) return PlatformTypes::Red;
        if (choice <= 1.0f) return PlatformTypes::Lime;
        return PlatformTypes::Blue;
    };

    short column = genColumn();
    float length = m_Random.Range(Constants::MinLength, Constants::MaxLength);
    short temp = column;
    PlatformType color = genColor();
    if (color == PlatformTypes::Red) {
        m_Platforms.emplace_back(
                genPos(column,zSpawnCoord, length), m_Random.Range(Constants::MinWidth, Constants::MaxWidth), length, PlatformTypes::Blue, m_PlatformsVelocity
        );
        column = m_LastColNum;
    }
    m_Platforms.emplace_back(
            genPos(column, zSpawnCoord, length), m_Random.Range(Constants::MinWidth, Constants::MaxWidth), length, color, m_PlatformsVelocity
    );
    m_LastColNum = temp;
}

void Platforms::SpawnPoint() {
    float maxZ = -1;
    for (auto& platform : m_Platforms) {
        Transform& transform = platform.GetTransform();
        maxZ = glm::max(maxZ, transform.position.z + transform.scales.z / 2);
    }
    if (maxZ < Constants::SpaceLength) {
        SpawnNewRow(Constants::SpaceLength);
    }
}

void Platforms::GarbagePoint() {
    auto itr = m_Platforms.begin();
    while (itr != m_Platforms.end()) {
        Transform& transform = itr->GetTransform();
        if (transform.position.z + transform.scales.z / 2 >= -Constants::MaxLength) {
            break;
        }
        itr++;
        m_Platforms.pop_front();
    }
}

void Platforms::SetGenerator() {
    m_Platforms.emplace_back(
            glm::vec3(0, 0, Constants::MaxLength / 2),
            Constants::MaxWidth, Constants::MaxLength, PlatformTypes::Blue , m_PlatformsVelocity
            );
    m_LastColNum = Constants::ColNum / 2;
    float lastZ = Constants::MaxLength;
    while (lastZ <= Constants::SpaceLength) {
        SpawnNewRow(lastZ);
        Transform& transform = m_Platforms.back().GetTransform();
        lastZ = transform.position.z + transform.scales.z / 2;
    }
}



