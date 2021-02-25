#include "Sandbox/Entities/Player.h"
#include "Sandbox/Managers/GPUManagers.h"
#include "Sandbox/Managers/Bonuses.h"

const float Player::s_MinForwardVelocity = 6.0f;
const float Player::s_MaxForwardVelocity = 20.0f;

Player::Player(const Transform &transform, Platforms *platforms)
: SpaceShip(transform), m_DownPlane(glm::vec3(0.0f), 0.4f),
    m_Sphere(transform.position, transform.scales.x), m_Platforms(platforms),
    m_Light(glm::vec3(1.5f, 1.5f, 1.5f), transform.position, glm::vec3(0, 0, -1.0f), glm::cos(glm::radians(20.0f))) {
    m_DownPlane.SetPosition(transform.position - glm::vec3(0, transform.scales.y, 0));
    m_Transform.rotations.y = glm::pi<float>();
    m_Transform.scales = glm::vec3(0.7f);
    m_Transform.ComputeModel();
    LightMaster::AddNewLight(&m_Light);
}

void Player::CollisionsUpdate() {
    if (m_State & Player::State::Destroyed)
        return;

    for (auto& platform : m_Platforms->GetPlatforms()) {
        if (!(m_State & Player::State::Invulnerable) && platform.HaveObstacle()
            && Hitbox::CollisionDetector::Detect(platform.GetObstacle().GetAABB(), m_Sphere)) {
            m_Life = glm::max(0, m_Life - 1);
            m_State |= Player::State::Invulnerable;
            m_InvulnerableTime = 0.5f;
        }
    }
    m_GravityAcceleration = 25.0f;
    for (auto& platform : m_Platforms->GetPlatforms()) {
        auto& rectPlane = platform.GetUpperPlane();
        if (Hitbox::CollisionDetector::Detect(rectPlane, m_DownPlane)) {
            if (platform.HasBuff()) {
                ApplyBuff(platform.GetType());
                platform.ConsumeBuff();
            }

            m_Velocity.y = m_GravityAcceleration = 0;
            m_Transform.position.y = rectPlane.GetPosition().y + m_Transform.scales.y;
            break;
        }
    }
}

void Player::Update(float deltaTime, WindowObject *window) {
    if (!(m_State & (Player::State::Destroying | Player::State::Destroyed))) {
        InputUpdate(deltaTime, window);
        m_Velocity.y -= deltaTime * m_GravityAcceleration;
        m_Transform.position.y += deltaTime * m_Velocity.y;
        m_Fuel = glm::max(0.0f, m_Fuel - 3 * deltaTime);
        if (m_Fuel == 0.0f) {
            m_Life--;
            m_Fuel = 100.0f;
        }
        if (m_State & Player::State::Invulnerable) {
            m_InvulnerableTime -= deltaTime;
            if (m_InvulnerableTime <= 0.0f) {
                m_State &= ~Player::State::Invulnerable;
            }
        }
        if (m_Life <= 0 || m_Fuel <= 0 || m_Transform.position.y < -10.0f) {
            m_State = Player::State::Destroying;
        }

        if (m_State & Player::State::HighSpeed) {
            m_HighSpeedTime -= deltaTime;
            m_Platforms->SetSpeed(s_MaxForwardVelocity);
            Bonuses::SetVelocity(glm::vec3(0, 0, -s_MaxForwardVelocity));
            if (m_HighSpeedTime <= 0.0f) {
                m_State &= ~Player::State::HighSpeed;
            }
        }
        else {
            m_Platforms->SetSpeed(m_Velocity.z);
            Bonuses::SetVelocity(glm::vec3(0, 0, -m_Velocity.z));
        }
        SpaceShip::Update(deltaTime, window);
    }
    else if (m_State & Player::State::Destroying) {
        m_Transform.scales -= 3 * deltaTime;
        if (m_Transform.scales.x <= 0.0f) {
            m_State = Player::State::Destroyed;
        }
    }
    else if (m_State & Player::State::Destroyed) {
        Bonuses::SetVelocity(glm::vec3(0, 0, -4.0f));
        m_Platforms->SetSpeed(4.0f);
    }

    m_DownPlane.SetPosition(m_Transform.position - glm::vec3(0, m_Transform.scales.y, 0));
    m_Sphere.SetPosition(m_Transform.position);
    m_Transform.ComputeModel();
    SpaceShip::Update(deltaTime, window);
    m_Light.Position = m_Transform.position + glm::vec3(0, -0.15f, 0.5f);
}

void Player::Render(const glm::mat4 &projection, const glm::mat4 &view) {
    if (!m_ThirdPerson)
        return;

    if (!(m_State & Player::State::Destroyed)) {
        if (m_State & Player::State::Destroying) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPointSize(3.0f);
        }
        else if (m_State & (Player::State::Invulnerable)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPointSize(2.0f);
        }

        SpaceShip::Render(projection, view);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPointSize(1.0f);
    }
}

void Player::ApplyBuff(PlatformType platformColor) {
    if (platformColor == PlatformTypes::Green) {
        m_Fuel = glm::min(m_Fuel + 30.0f, m_MaxFuel);
    }
    else if (platformColor == PlatformTypes::Yellow) {
        m_Fuel = glm::max(m_Fuel - 10.0f, 0.0f);
    }
    else if (platformColor == PlatformTypes::Red && !(m_State & Player::State::Invulnerable)) {
        m_State = Player::State::Destroying;
    }
    else if (platformColor == PlatformTypes::Orange) {
        m_State |= Player::State::HighSpeed;
        m_HighSpeedTime = 2.0;
    }
    else if (platformColor == PlatformTypes::Lime) {
        m_State |= Player::State::Invulnerable;
        m_InvulnerableTime = 5.0f;
    }
}

void Player::InputUpdate(float deltaTime, WindowObject *window) {
    constexpr float zSpeed = 10.0f;
    if (m_Velocity.y == 0) {
        if (window->KeyHold(GLFW_KEY_SPACE)) {
            m_Velocity.y = 12.0f;
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            m_Velocity.z += zSpeed * deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            m_Velocity.z -= zSpeed * deltaTime;
        }
    }
    m_Velocity.z = glm::min(glm::max(m_Velocity.z, s_MinForwardVelocity), s_MaxForwardVelocity);

    constexpr float xSpeed = 4.0f;
    constexpr float rotationSpeed = 4.0f;
    m_Transform.rotations.x += rotationSpeed * deltaTime;
    if (window->KeyHold(GLFW_KEY_A)) {
        m_Transform.position.x += xSpeed * deltaTime;
        m_Transform.rotations.z += deltaTime * rotationSpeed;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        m_Transform.position.x -= xSpeed * deltaTime;
        m_Transform.rotations.z -= deltaTime * rotationSpeed;
    }
}






