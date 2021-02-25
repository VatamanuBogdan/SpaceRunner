#pragma once
#include "Sandbox/Entities/Decorations/LightSource.h"
#include "Sandbox/Entities/Decorations/Satellite.h"
#include "Sandbox/Entities/Decorations/SpaceShip.h"
#include "Sandbox/Entities/Decorations/Ufo.hpp"
#include "Sandbox/Entities/Decorations/SolarSystem.h"
#include "Sandbox/Utility/Randomizer.hpp"

#include <deque>
#include <numeric>

class Decorations {
public:
    Decorations()
    : m_Random(),
        m_SolarSystem(Transform(glm::vec3(30.0f, 5.0f, 40.0f), glm::vec3(0, 0, glm::pi<float>() / 6), glm::vec3(0.55f))) {
        SpawnState();
    }

    void Update(float deltaTime, WindowObject* window) {
        GarbageState();
        SpawnState();
        for (auto& light : m_Lights) {
            light.GetTransform().position += deltaTime * m_Speed;
            light.Update(deltaTime, window);
        }
        for (auto& ufo : m_UFOs) {
            ufo.GetTransform().position += deltaTime * m_Speed;
            ufo.Update(deltaTime, window);
        }
        for (auto& satellite : m_Satellites) {
            satellite.GetTransform().position += deltaTime * m_Speed;
            satellite.Update(deltaTime, window);
        }
        for (auto& spaceShip : m_SpacesShips) {
            spaceShip.GetTransform().position += deltaTime * m_Speed;
            spaceShip.Update(deltaTime, window);
        }
        m_SolarSystem.Update(deltaTime, window);
    }

    void Render(const glm::mat4 &proj, const glm::mat4& view) {
        for (auto& light : m_Lights) light.Render(proj, view);
        for (auto& ufo : m_UFOs) ufo.Render(proj, view);
        for (auto& satellite : m_Satellites) satellite.Render(proj, view);
        for (auto& spaceShip : m_SpacesShips) spaceShip.Render(proj, view);
        m_SolarSystem.Render(proj, glm::mat3(view));
    }

    void SetVelocity(const glm::vec3 &velocity) { m_Speed = velocity; }

private:
    void SpawnNewDeco(uint8_t lane) {
        glm::vec3 pos;
        pos.y = m_Random() * (HigherY - LowerY) + LowerY;
        if (lane == 0) {
            pos.x = (LeftHigherX - LeftLowerX) * m_Random() + LeftLowerX;
            pos.z = m_LeftLastZ + Distance;
            m_LeftLastZ = pos.z;
        } else {
            pos.x = (RightHigherX - RightLowerX) * m_Random() + RightLowerX;
            pos.z = m_RightLastZ + Distance;
            m_RightLastZ = pos.z;
        }
        float rand = m_Random();
        float timeAcc = m_Random() * 500.0f;
        if (rand <= 0.35f) {
            m_UFOs.emplace_back(Transform(pos, glm::vec3(0), glm::vec3(2.0f)), timeAcc);
        } else if (rand <= 0.65f) {
            m_Satellites.emplace_back(Transform(pos, glm::vec3(0), glm::vec3(1.5f)), timeAcc);
        } else if (rand <= 0.90f) {
            m_SpacesShips.emplace_back(Transform(pos, glm::vec3(0), glm::vec3(4.0f)), timeAcc);
        } else if (rand <= 1.0f) {
            m_Lights.emplace_back(Transform(pos, glm::vec3(0), glm::vec3(0.75f)), timeAcc);
        }
    }

    void SpawnState() {
        while (m_LeftLastZ < HigherZ) {
            SpawnNewDeco(0);
        }
        while (m_RightLastZ < HigherZ) {
            SpawnNewDeco(1);
        }
    }

    template <typename T>
    void GarbageContainer(std::deque<T> &container) {
        auto itr = std::remove_if(container.begin(), container.end(),
                                  [] (T& entry) { return entry.GetTransform().position.z < LowerZ; } );
        container.erase(itr, container.end());
    }

    template <typename T>
    void FindLastZ(std::deque<T> &container) {
        for (T& elem : container) {
            glm::vec3& pos = elem.GetTransform().position;
            if (pos.x >= LeftLowerX && pos.x <= LeftHigherX) {
                m_LeftLastZ = glm::max(pos.z, m_LeftLastZ);
            } else {
                m_RightLastZ = glm::max(pos.z, m_RightLastZ);
            }
        }
    }

    void GarbageState() {
        GarbageContainer(m_Lights);
        GarbageContainer(m_Satellites);
        GarbageContainer(m_SpacesShips);
        GarbageContainer(m_UFOs);
        m_LeftLastZ = m_RightLastZ = 0.0f;
        FindLastZ(m_Lights);
        FindLastZ(m_Satellites);
        FindLastZ(m_SpacesShips);
        FindLastZ(m_UFOs);
    }

private:
    static constexpr float Distance = 50.0f;
    static constexpr float HigherZ = 150.0f;
    static constexpr float LowerZ = -20.0f;
    static constexpr float LeftLowerX = -17.0f;
    static constexpr float LeftHigherX = -12.0f;
    static constexpr float RightLowerX = 12.0f;
    static constexpr float RightHigherX = 17.0f;
    static constexpr float LowerY = 5.0f;
    static constexpr float HigherY = 8.0f;

private:
    float m_LeftLastZ = 0.0f;
    float m_RightLastZ = 0.0f;
    glm::vec3 m_Speed = glm::vec3(0, 0, -2.0f);
    Randomizer m_Random;

private:
    std::deque<LightSource> m_Lights;
    std::deque<Satellite> m_Satellites;
    std::deque<SpaceShip> m_SpacesShips;
    std::deque<Ufo> m_UFOs;
    SolarSystem m_SolarSystem;


};