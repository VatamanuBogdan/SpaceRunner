#pragma once
#include "Sandbox/Entities/Entity.h"

#include <Core/Engine.h>

namespace Hitbox {

    class Shape3D {
    protected:
        explicit Shape3D(const glm::vec3& position)
        : m_Position(position) {
        }

    public:
        void SetPosition(const glm::vec3& position) { m_Position = position; }
        const glm::vec3& GetPosition() const { return m_Position; }

    protected:
        glm::vec3 m_Position;
    };



    class AABB : public Shape3D {
    public:
        AABB(const glm::vec3& position, float xEdge, float yEdge, float zEdge)
        : Shape3D(position), m_XEdge(xEdge), m_YEdge(yEdge), m_ZEdge(zEdge) {
        }

        void SetXEdge(float xEdge) { m_XEdge = xEdge; }
        void SetYEdge(float yEdge) { m_YEdge = yEdge; }
        void SetZEdge(float zEdge) { m_ZEdge = zEdge; }

        float GetXEdge() const { return m_XEdge; }
        float GetYEdge() const { return m_YEdge; }
        float GetZEdge() const { return m_ZEdge; }

    private:
        float m_XEdge;
        float m_YEdge;
        float m_ZEdge;
    };


    class Sphere : public Shape3D {
    public:
        Sphere(const glm::vec3& position, float radius)
        : Shape3D(position), m_Radius(radius) {
        }

        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }

    protected:
        float m_Radius;
    };


    class RectanglePlane : public Shape3D {
    public:
        RectanglePlane(const glm::vec3& position, float width, float height)
        : Shape3D(position), m_Width(width), m_Height(height) {
        }

        void SetWidth(float width) { m_Width = width; }
        float GetWidth() const { return m_Width; }


        void SetHeight(float height) { m_Height = height; }
        float GetHeight() const { return m_Height; }

    protected:
        float m_Width;
        float m_Height;
    };


    class CirclePlane : public Shape3D {
    public:
        CirclePlane(const glm::vec3 &position, float radius)
        : Shape3D(position), m_Radius(radius) {
        }

        void SetRadius(float radius) { m_Radius = radius; }
        float GetRadius() const { return m_Radius; }

    protected:
        float m_Radius;
    };

    class CollisionDetector {
    public:
        CollisionDetector() = delete;

        static bool Detect(const RectanglePlane& rectangle, const CirclePlane& circle);
        static bool Detect(const AABB& aabb, const Sphere& sphere);

    };

};