#include "Sandbox/Mechanics/Collisions.h"

bool Hitbox::CollisionDetector::Detect(const RectanglePlane &rectangle, const CirclePlane &circle) {
    const glm::vec3& rectPos = rectangle.GetPosition();
    const glm::vec3& circlePos = circle.GetPosition();
    if (glm::abs(rectPos.y - circlePos.y) > 0.1f)
        return false;

    float xWidth = rectangle.GetWidth();
    float zHeight = rectangle.GetHeight();

    float xMin = rectPos.x - xWidth / 2, xMax = rectPos.x + xWidth / 2;
    float zMin = rectPos.z - zHeight / 2, zMax = rectPos.z + zHeight / 2;

    float x = glm::max(xMin, glm::min(circlePos.x, xMax));
    float z = glm::max(zMin, glm::min(circlePos.z, zMax));
    float distance = glm::sqrt(
            (x - circlePos.x) * (x - circlePos.x) + (z - circlePos.z) * (z - circlePos.z)
    );
    return distance < circle.GetRadius();
}

bool Hitbox::CollisionDetector::Detect(const Hitbox::AABB &aabb, const Hitbox::Sphere &sphere) {
    const glm::vec3& aabbPos = aabb.GetPosition();
    const glm::vec3& spherePos = sphere.GetPosition();

    float xEdge = aabb.GetXEdge();
    float yEdge = aabb.GetYEdge();
    float zEdge = aabb.GetZEdge();

    float xMin = aabbPos.x - xEdge / 2, xMax = aabbPos.x + xEdge / 2;
    float yMin = aabbPos.y - yEdge / 2, yMax = aabbPos.y + yEdge / 2;
    float zMin = aabbPos.z - zEdge / 2, zMax = aabbPos.z + zEdge / 2;

    float x = glm::max(xMin, glm::min(spherePos.x, xMax));
    float y = glm::max(yMin, glm::min(spherePos.y, yMax));
    float z = glm::max(zMin, glm::min(spherePos.z, zMax));
    float distance = glm::sqrt(
            (x - spherePos.x) * (x - spherePos.x) +
                (y - spherePos.y) * (y - spherePos.y) +
                (z - spherePos.z) * (z - spherePos.z)
    );
    return distance < sphere.GetRadius();
}

