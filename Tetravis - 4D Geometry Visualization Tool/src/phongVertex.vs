// Based on work by Joey de Vries© (https://twitter.com/JoeyDeVriez)
// Website: https://learnopengl.com/
// Source: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/2.2.basic_lighting_specular/2.2.basic_lighting.vs
// License: https://creativecommons.org/licenses/by-nc/4.0/
// Modifications by Alexia Soucy, 2020

#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(inPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * inNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}