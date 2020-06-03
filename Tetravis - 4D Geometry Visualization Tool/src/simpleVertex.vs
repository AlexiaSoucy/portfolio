// Based on work by Joey de Vries© (https://twitter.com/JoeyDeVriez)
// Website: https://learnopengl.com/
// Source: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/3.3.shaders_class/3.3.shader.vs
// License: https://creativecommons.org/licenses/by-nc/4.0/
// Modifications by Alexia Soucy, 2020

#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
}