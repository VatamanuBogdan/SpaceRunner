#pragma once
#include <Engine.h>
#include <unordered_map>
#include <functional>

class MeshesManager {
public:
    MeshesManager() = delete;

    static void Init();
    static Mesh* Get(const char* identifier);
    static void Free();
private:
    static void LoadMesh(const char* identifier, const char* fileName);

private:
    static std::unordered_map<std::string, Mesh*> s_Meshes;
};


class ShadersManager {
public:
    ShadersManager() = delete;

    static void Init();
    static Shader* Get(const char* name);
    static void Free();

private:
    static void LoadShader(const char* name, const char* vertexName, const char* fragmentName);

private:
    static std::unordered_map<std::string, Shader*> s_Shaders;
};


class TexturesManager {
public:
    TexturesManager() = delete;

    static void Init();
    static Texture2D* Get(const char* name);
    static void Free();

private:
    static void LoadTexture(const char* id, const char* fileName);

private:
    static std::unordered_map<std::string, Texture2D*> s_Textures;
};

