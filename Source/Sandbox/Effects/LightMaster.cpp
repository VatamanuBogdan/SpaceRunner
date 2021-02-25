#include "LightMaster.h"
#include "Sandbox/Managers/GPUManagers.h"

#include <cstdio>
#include <iostream>

void LightMaster::Init() {
}

void LightMaster::AddNewLight(SimpleLight *light) {
    if (s_PointLights.size() < MaxSimpleLights)
        s_PointLights.insert(light);
}

void LightMaster::AddNewLight(SpotLight *light) {
    if (s_SpotLights.size() < MaxSpotLights)
        s_SpotLights.insert(light);
}

void LightMaster::RemoveLight(SimpleLight *light) {
    s_PointLights.erase(light);
}

void LightMaster::RemoveLight(SpotLight *light) {
    s_SpotLights.erase(light);
}

void LightMaster::ApplyLights(const glm::vec3 &cameraPosition) {
    Shader* shader = ShadersManager::Get("Textured");
    LightMaster::ApplyAmbientLight(shader, cameraPosition);
    LightMaster::ApplyPointLights(shader);
    LightMaster::ApplySpotLights(shader);
}

void LightMaster::SetAmbient(const AmbientLight &light) { s_AmbientLight = light; }

void LightMaster::ApplyAmbientLight(Shader *shader, const glm::vec3 &cameraPosition) {
    shader->Use();
    glUniform3f(shader->GetUniformLocation("ViewPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glUniform3f(shader->GetUniformLocation("Ambiental.color"), s_AmbientLight.Color.x, s_AmbientLight.Color.y, s_AmbientLight.Color.z);
    glUniform1f(shader->GetUniformLocation("Ambiental.strenght"), s_AmbientLight.Strength);
}

void LightMaster::ApplyPointLights(Shader* shader) {
    shader->Use();
    uint32_t i = 0;
    char tempBuff[256];
    for (auto entry = s_PointLights.begin(); entry != s_PointLights.end() && i < MaxSimpleLights; entry++, i++) {
        sprintf(tempBuff, "pointLights[%d].color", i);
        glm::vec3& color = (*entry)->Color;
        glUniform3f(shader->GetUniformLocation(tempBuff), color.x, color.y, color.z);
        sprintf(tempBuff, "pointLights[%d].position", i);
        glm::vec3& position = (*entry)->Position;
        glUniform3f(shader->GetUniformLocation(tempBuff), position.x, position.y, position.z);
    }
    while (i < MaxSimpleLights) {
        sprintf(tempBuff, "pointLights[%d].position", i);
        glUniform3f(shader->GetUniformLocation(tempBuff), 0.0f, 0.0f, 0.0f);
        sprintf(tempBuff, "pointLights[%d].color", i);
        glUniform3f(shader->GetUniformLocation(tempBuff), 0.0f, 0.0f, 0.0f);
        i++;
    }
}

void LightMaster::ApplySpotLights(Shader* shader) {
    shader->Use();
    uint32_t i = 0;
    char tempBuff[256];
    for (auto entry = s_SpotLights.begin(); entry != s_SpotLights.end() && i < MaxSpotLights; entry++, i++) {
        sprintf(tempBuff, "spotLights[%d].color", i);
        glm::vec3& color = (*entry)->Color;
        glUniform3f(shader->GetUniformLocation(tempBuff), color.x, color.y, color.z);
        sprintf(tempBuff, "spotLights[%d].position", i);
        glm::vec3& position = (*entry)->Position;
        glUniform3f(shader->GetUniformLocation(tempBuff), position.x, position.y, position.z);
        sprintf(tempBuff, "spotLights[%d].direction", i);
        glm::vec3& direction = (*entry)->direction;
        glUniform3f(shader->GetUniformLocation(tempBuff), direction.x, direction.y, direction.z);
        sprintf(tempBuff, "spotLights[%d].cutOff", i);
        glUniform1f(shader->GetUniformLocation(tempBuff), (*entry)->cutOff);
    }
    while (i < MaxSpotLights) {
        sprintf(tempBuff, "spotLights[%d].position", i);
        glUniform3f(shader->GetUniformLocation(tempBuff), 0.0f, 0.0f, 0.0f);
        sprintf(tempBuff, "spotLights[%d].color", i);
        glUniform3f(shader->GetUniformLocation(tempBuff), 0.0f, 0.0f, 0.0f);
        i++;
    }
}

std::set<SimpleLight*> LightMaster::s_PointLights;
std::set<SpotLight*> LightMaster::s_SpotLights;
AmbientLight LightMaster::s_AmbientLight = AmbientLight(glm::vec3(1.0, 1.0, 1.0), 0.15f);


