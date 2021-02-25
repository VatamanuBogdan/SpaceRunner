#include "Bonuses.h"

void Bonuses::AddBonus(const Bonus &bonus) {
    s_Bonuses.push_back(bonus);
}

float Bonuses::CheckCollision(Player &player) {
    float points = 0.0f;
    for (auto &bonus : s_Bonuses) {
        if (!bonus.IsCollected() && Hitbox::CollisionDetector::Detect(bonus.GetHitbox(), player.GetSphere())) {
            bonus.Collect();
            points += 1.0f;
        }
    }
    return points;
}

void Bonuses::SetVelocity(const glm::vec3 &velocity) { s_Velocity = velocity; }

void Bonuses::Update(float deltaTime, WindowObject *window) {
    for (auto &bonus : s_Bonuses) {
        bonus.SetSpeed(s_Velocity);
        bonus.Update(deltaTime, window);
    }
    s_Bonuses.erase(std::remove_if(s_Bonuses.begin(), s_Bonuses.end(), [] (Bonus& bonus) {
        return bonus.GetTransform().position.z < -10.0f || bonus.IsDestroyed();
    }), s_Bonuses.end());
}

void Bonuses::Render(const glm::mat4 &proj, const glm::mat4 &view) {
    for (auto &bonus : s_Bonuses) { bonus.Render(proj, view); }
}

std::deque<Bonus> Bonuses::s_Bonuses;
glm::vec3 Bonuses::s_Velocity = glm::vec3(0.0f);
