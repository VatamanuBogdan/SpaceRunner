#include "GPUManagers.h"
#include "Sandbox/Utility/Paths.h"

#include <iostream>

void MeshesManager::Init() {
    LoadMesh("Cube", "Cube.obj");
    LoadMesh("Sphere", "Sphere.obj");
    LoadMesh("Quad", "Quad.obj");
    LoadMesh("Cone", "Cone.obj");
    LoadMesh("Hemisphere", "Hemisphere.obj");
    LoadMesh("Cylinder", "Cylinder.obj");
    LoadMesh("Player", "Player.obj");
}

Mesh* MeshesManager::Get(const char* identifier) {
    auto entry = s_Meshes.find(identifier);
    return entry != s_Meshes.end() ? entry->second : nullptr;
}

void MeshesManager::Free() {
    for (auto& entry : s_Meshes) delete entry.second;
}

void MeshesManager::LoadMesh(const char* identifier, const char* fileName) {
    Mesh* mesh = new Mesh("");
    mesh->LoadMesh(Resources::ModelPath, fileName);
    s_Meshes.insert({identifier, mesh});
}

std::unordered_map<std::string, Mesh*> MeshesManager::s_Meshes;


void ShadersManager::Init() {
    LoadShader("Textured", "Textured.VS.glsl", "Textured.FS.glsl");
    LoadShader("Hud", "Hud.VS.glsl", "Hud.FS.glsl");
    LoadShader("Blur", "Blur.VS.glsl", "Blur.FS.glsl");
    LoadShader("Light", "Light.VS.glsl", "Light.FS.glsl");
    LoadShader("Skybox", "Skybox.VS.glsl", "Skybox.FS.glsl");
}

Shader* ShadersManager::Get(const char* name) {
    auto entry = s_Shaders.find(name);
    return entry != s_Shaders.end() ? entry->second : nullptr;
}

void ShadersManager::Free() {
    for (auto& entry : s_Shaders) delete entry.second;
}

void ShadersManager::LoadShader(const char* name, const char *vertexName, const char *fragmentName) {
    Shader* shader;
    shader = new Shader("");
    shader->AddShader(Resources::ShaderPath + vertexName, GL_VERTEX_SHADER);
    shader->AddShader(Resources::ShaderPath + fragmentName, GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    s_Shaders.insert({name, shader});
}

std::unordered_map<std::string, Shader*> ShadersManager::s_Shaders;


void TexturesManager::Init() {
    TexturesManager::LoadTexture("Earth", "earth.png");
    TexturesManager::LoadTexture("Moon", "moon.png");
    TexturesManager::LoadTexture("Sun", "sun.jpg");
    TexturesManager::LoadTexture("Venus", "venus.png");
    TexturesManager::LoadTexture("Metallic", "metallic.jpg");
    TexturesManager::LoadTexture("Green", "green.jpg");
    TexturesManager::LoadTexture("Purple", "purple.png");
    TexturesManager::LoadTexture("GrayMetal", "gray_metal.jpg");
    TexturesManager::LoadTexture("RedMetal", "red_metal.jpg");
    TexturesManager::LoadTexture("SolarPanel", "solar_panel.jpg");
    TexturesManager::LoadTexture("GraySteel", "gray_steel.jpg");
    TexturesManager::LoadTexture("BlueGlass", "blue_glass.jpg");
    TexturesManager::LoadTexture("Rust", "rust.jpg");
    TexturesManager::LoadTexture("BluePlatform", "blue_platform.png");
    TexturesManager::LoadTexture("RedPlatform", "red_platform.png");
    TexturesManager::LoadTexture("YellowPlatform", "yellow_platform.png");
    TexturesManager::LoadTexture("OrangePlatform", "orange_platform.png");
    TexturesManager::LoadTexture("GreenPlatform", "green_platform.png");
    TexturesManager::LoadTexture("PurplePlatform", "purple_platform.png");
    TexturesManager::LoadTexture("LimePlatform", "lime_platform.png");
    TexturesManager::LoadTexture("BlueLiquid", "blue_liquid.jpg");
    TexturesManager::LoadTexture("Box", "box.png");
}

Texture2D* TexturesManager::Get(const char* name) {
    auto entry = s_Textures.find(name);
    return entry != s_Textures.end() ? entry->second : nullptr;
}

void TexturesManager::Free() {
    for (auto& entry : s_Textures) delete entry.second;
}

void TexturesManager::LoadTexture(const char *id, const char *fileName) {
    auto *texture = new Texture2D();
    texture->Load2D((Resources::TexturePath + fileName).c_str());
    s_Textures.insert({id, texture});
}

std::unordered_map<std::string, Texture2D*> TexturesManager::s_Textures;