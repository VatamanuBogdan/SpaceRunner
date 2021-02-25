#version 330

uniform sampler2D texture_1;

in vec3 f_position;
in vec3 f_normal;
in vec2 tex_coord;
in vec3 view_dir;

struct AmbientalLight {
    float strenght;
    vec3 color;
};

struct PointLight {
    vec3 color;
    vec3 position;
};

struct SpotLight {
    vec3 color;
    vec3 position;
    vec3 direction;
    float cutOff;
};

const float constant = 1.0f;
const float linear = 0.0014f;
const float quadratic = 0.000007f;
const float shininess = 32.0f;


float DiffuseSpecularPoint(PointLight light) {
    vec3 light_dir = normalize(light.position - f_position);
    vec3 reflect_dir = reflect(-light_dir, f_normal);
    float diffuse = max(dot(f_normal, light_dir), 0.0);
    float specular = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
    float distance = length(light.position - f_position);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    return  attenuation * (diffuse + specular);
}

float DiffuseSpecularSpot(SpotLight light) {
    float theta = dot(normalize(light.position - f_position), light.direction);
    if (theta < light.cutOff)
        return 0.0f;
    return DiffuseSpecularPoint(PointLight(light.color, light.position));
}

#define POINT_LIGHT_NUM 15
#define SPOT_LIGHT_NUM 15

uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLights[SPOT_LIGHT_NUM];
uniform AmbientalLight Ambiental;

layout(location = 0) out vec4 out_color;

void main() {
    vec4 object_color = texture2D(texture_1, tex_coord);
    vec3 light = Ambiental.color * Ambiental.strenght;
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
        light += pointLights[i].color * DiffuseSpecularPoint(pointLights[i]);
    for (int i = 0; i < SPOT_LIGHT_NUM; i++)
        light += spotLights[i].color * DiffuseSpecularSpot(spotLights[i]);
    out_color = vec4(object_color.xyz * light, object_color.w);
}