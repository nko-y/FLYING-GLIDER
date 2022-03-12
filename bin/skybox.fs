#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    vec3 color = vec3(0.604f, 0.655f, 0.718f);
    FragColor = vec4(mix(texture(skybox, TexCoords).rgb, color, 0.50f), 1.0f);
}