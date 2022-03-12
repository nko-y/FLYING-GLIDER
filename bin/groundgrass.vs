#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Light {
    vec3 direction;
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D ambient;
    sampler2D normal;
    sampler2D displacement;
    float shininess;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform mat4 lightSpaceMatrix;

void main()
{
	TexCoords = aTexCoords;

    Normal = mat3(transpose(inverse(model))) * aNormal;  

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    //float scaleSize = 0.0;
    //vec3 disnormal = texture(material.normal, TexCoords).rgb;
    //disnormal = normalize(disnormal * 2.0 - 1.0);
    //disnormal = mat3(T, B, N) * disnormal;
    //vec3 disPos = aPos + scaleSize * texture(material.displacement, TexCoords).r * disnormal;
    vec3 disPos = aPos;

    FragPos = vec3(model * vec4(disPos, 1.0));

    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * (FragPos+light.direction);
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}