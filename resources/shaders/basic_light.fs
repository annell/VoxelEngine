#version 330 core
out vec4 FragColor;

#define MAXLIGHTS 10
#define MAXMATERIALS 128

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 lightPos;
    vec3 lightColor;

    int type;

    float constant;
    float linear;
    float quadratic;
};


in vec3 Normal;
in vec3 FragPos;
flat in int MaterialIndex;

uniform int nrLights;
uniform Light lights[MAXLIGHTS];
uniform Material materials[MAXMATERIALS];
uniform vec3 viewPos;

vec3 ambientLight(vec3 lightPos, vec3 lightColor) {
    // ambient
    int i = MaterialIndex;
    vec3 ambient = lightColor * materials[i].ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * materials[i].diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materials[i].shininess);
    vec3 specular = lightColor * (spec * materials[i].specular);

    return ambient + diffuse + specular;
}

float getAttenuation(vec3 lightPos, float constant, float linear, float quadratic) {
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + 
                    quadratic * (distance * distance)); 
    return attenuation;
}

vec3 processLight(Light light) {
    vec3 result = ambientLight(light.lightPos, light.lightColor);
    if (light.type == 1) {
        result *= getAttenuation(
            light.lightPos,
            light.constant,
            light.linear,
            light.quadratic
        );
    }
    return result;
}

void main() {
    vec3 result = processLight(lights[0]);
    for (int i = 1; i < nrLights; i++) {
        result += processLight(lights[i]);
    }
    FragColor = vec4(result, 1.0);
}