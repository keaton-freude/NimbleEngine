#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_map;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool lightingEnabled;
uniform float UvMultiplier;

float shadow_calculation(vec4 fragPosLightSpace, float bias) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) {
        return 0.0;
    }

    float closestDepth = texture(shadow_map, projCoords.xy).r;

    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0f;

    return shadow;
}

void main()
{
    if (lightingEnabled) {
        vec3 color = texture(diffuse_texture, TexCoord * UvMultiplier).rgb;
        vec3 normal = normalize(Normal);
        vec3 lightColor = vec3(0.3);
        vec3 ambient = 0.2 * color;

        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = spec * lightColor;

        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
        float shadow = shadow_calculation(FragPosLightSpace, bias);

        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

        FragColor = vec4(lighting, 1.0);
    } else {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture2D(diffuse_texture, TexCoord * UvMultiplier);
    }
}