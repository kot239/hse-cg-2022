#version 330 core

// in vec3 viewPos;
in vec3 fragPos;
in vec3 rawNormal;

uniform vec3 lightPos;

out vec4 out_col;

void main()
{
    vec3 objColor = vec3(1.0f, 0.0f, 0.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse

    vec3 normalizedNorm = normalize(rawNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(lightDir, normalizedNorm), 0.0f);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(- fragPos);
    vec3 reflectDir = reflect(-lightDir, normalizedNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;

    out_col = vec4(result.xyz, 1.0f);
}