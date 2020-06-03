// Based on work by Joey de Vries© (https://twitter.com/JoeyDeVriez)
// Website: https://learnopengl.com/
// Source: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/3.3.shaders_class/3.3.shader.fs
// License: https://creativecommons.org/licenses/by-nc/4.0/
// Modifications by Alexia Soucy, 2020

#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
	FragColor = vec4(objectColor, 1.0);
}