#version 330 core
out vec4 FragColor;

#define MAXLIGHTS 128
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
};


in vec3 Normal;
in vec3 FragPos;
flat in int MaterialIndex;

uniform int nrLights;
uniform Light lights[MAXLIGHTS];
uniform Material materials[MAXMATERIALS];
uniform vec3 viewPos;

vec3 calculateLight(vec3 lightPos, vec3 lightColor) {
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

void main()
{
    vec3 result = calculateLight(lights[0].lightPos, lights[0].lightColor);
    for (int i = 1; i < nrLights; i++) {
        vec3 light = calculateLight(lights[i].lightPos, lights[i].lightColor);
        result += light;
    }
    FragColor = vec4(result, 1.0);
}