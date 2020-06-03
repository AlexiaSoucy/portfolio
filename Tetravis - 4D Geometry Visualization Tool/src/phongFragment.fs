// Based on work by Joey de Vries© (https://twitter.com/JoeyDeVriez)
// Website: https://learnopengl.com/
// Source: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/2.2.basic_lighting_specular/2.2.basic_lighting.fs
// License: https://creativecommons.org/licenses/by-nc/4.0/
// Modifications by Alexia Soucy, 2020

#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // If normals point away from the camera, flip them.
    vec3 fixedNorm = Normal;
    if (fixedNorm.z < 0.0)
    {
        fixedNorm = vec3(-Normal.x, -Normal.y, -Normal.z);
	}

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(fixedNorm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewPos = vec3(0.0f, 0.0f, 5.0f);
    float specularStrength = 0.1;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 