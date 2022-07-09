#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in float materialIndex;

out vec3 FragPos;
out vec3 Normal;
flat out int MaterialIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = vec3(model * vec4(aNormal, 1.0));
    MaterialIndex = int(materialIndex);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}