#pragma once
#include "Sandbox/Entities/Bonus.hpp"
#include "Sandbox/Entities/Player.h"

#include <deque>

class Bonuses {
public:
    Bonuses() = delete;

    static void Init() {
    }

    static void AddBonus(const Bonus& bonus);
    static float CheckCollision(Player& player);

    static void SetVelocity(const glm::vec3 &velocity);

    static void Update(float deltaTime, WindowObject *window);
    static void Render(const glm::mat4 &proj, const glm::mat4 &view);

private:
    static std::deque<Bonus> s_Bonuses;
    static glm::vec3 s_Velocity;
};