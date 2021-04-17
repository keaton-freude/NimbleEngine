#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform sampler2D diffuse_texture;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 viewPos;
uniform bool lightingEnabled;

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

        vec3 result = (ambient + diffuse + specular) * vec3(1.0, 1.0, 1.0);
        FragColor = vec4(result, 1.0) * texture2D(diffuse_texture, TexCoord);
    } else {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * texture2D(diffuse_texture, TexCoord);
    }
}