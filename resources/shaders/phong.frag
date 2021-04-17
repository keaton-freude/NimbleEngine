#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_map;

uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 viewPos;
uniform bool lightingEnabled;
uniform float UvMultiplier;

float shadow_calculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadow_map, projCoords.xy).r;

    float currentDepth = projCoords.z;

    //float bias = max(0.05 * (1.0 - dot(Normal, lightDirection)), 0.005);
    float bias = 0.001;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    if (lightingEnabled) {
        float ambientStrength = 0.6;
        vec3 ambient = ambientStrength * lightColor;

        vec3 normal = normalize(Normal);
        vec3 lightDir = normalize(lightDirection);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        float shadow = shadow_calculation(FragPosLightSpace);

        vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture2D(diffuse_texture, TexCoord * UvMultiplier).rgb;
        FragColor = vec4(result, 1.0);

    } else {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture2D(diffuse_texture, TexCoord * UvMultiplier);
    }
}