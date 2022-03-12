#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D ambient;
    sampler2D normal;
    sampler2D displacement;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Fog{
    float Density;
    vec3 Color;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;
in vec4 FragPosLightSpace;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform sampler2D shadowMap;
uniform Fog fog;

float ShadowCalculation(vec4 fragPosLightSpace)
{
  // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos - FragPos);
    float bias = max(0.0002 * (1.0 - dot(normal, lightDir)), 0.0001);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    //return 0.0;
    return shadow;
}

float getFogFactor(Fog fog, vec3 viewPos, vec3 fragPos){
    float distance = length(viewPos - fragPos);
    float factor = exp(-pow(fog.Density * distance, 2));
    factor = clamp(factor, 0.0, 1.0);
    return 1 - factor;
}

void main()
{
    // ambient
    vec3 ambient = 0.08 * light.ambient * texture(material.ambient, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = texture(material.normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(0.2f, 0.2f, 0.2f));  

 	float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    // vec3 colorambient = 0.08 * light.ambient;
    // vec3 normal = normalize(Normal);
    // float dotLight = dot(normal, light.direction);
    // vec3 colordiffuse = light.diffuse * max(dotLight, 0.0);
    // vec3 color = texture(groundgrass, TexCoords).rgb * (colorambient+colordiffuse * shadow);
    // color.x = pow(color.x, 1.0 / 2.2);
    // color.y = pow(color.y, 1.0 / 2.2);
    // color.z = pow(color.z, 1.0 / 2.2);

    lighting.x = pow(lighting.x, 1.0 / 2.2);
    lighting.y = pow(lighting.y, 1.0 / 2.2);
    lighting.z = pow(lighting.z, 1.0 / 2.2);

    // FragColor = vec4(shadow, shadow, shadow, 1.0);
    // FragColor = vec4(lighting, 1.0);

    float fogFactor = getFogFactor(fog, viewPos, FragPos);
    vec3 colorWithFog = mix(lighting, fog.Color, fogFactor);
    FragColor = vec4(colorWithFog, 1.0);
} 