#version 330 core
out vec4 FragColor;

#define MAXLIGHTS 128

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 lightPos;
    vec3 lightColor;
};

uniform Material material;

in vec3 Normal;
in vec3 FragPos;

uniform int nrLights;
uniform Light lights[MAXLIGHTS];
uniform vec3 viewPos;
uniform vec3 objectColor;

vec3 calculateLight(vec3 lightPos, vec3 lightColor) {
    // ambient
    vec3 ambient = lightColor * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 result = calculateLight(lights[0].lightPos, lights[0].lightColor);
    for (int i = 1; i < nrLights; i++) {
        vec3 light = calculateLight(lights[i].lightPos, lights[i].lightColor);
        result += light;
    }
    FragColor = vec4(result, 1.0);
}