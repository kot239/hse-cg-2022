#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform int time;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool isVertexLightning;

out vec3 rawPos;
out vec3 fragPos;
out vec3 rawNormal;

out vec3 ambientVertex;
out vec3 diffuseVertex;
out vec3 specularVertex;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    // morph coordinates
    float beta = 1.5 * M_PI * (pos.y + 0.22) / 0.4;

    vec3 endPos = vec3(pos.x * cos(beta) - pos.z * sin(beta), pos.y, pos.x * sin(beta) + pos.z * cos(beta));
    rawPos = mix(pos.xyz, endPos, sin(2 * M_PI * (time % 300) / 300.0));

    vec3 endNorm = vec3(norm.x * cos(beta) - norm.z * sin(beta), norm.y, norm.x * sin(beta) + norm.z * cos(beta));
    rawNormal = normalMatrix * mix(norm.xyz, endNorm, sin(2 * M_PI * (time % 300) / 300.0));

    fragPos = vec3(view * model * vec4(rawPos.xyz, 1.0));

    // calculate lightning parameters
    if (isVertexLightning) {
        // ambient
        float ambientStrength = 0.1;
        ambientVertex = ambientStrength * lightColor;

        // diffuse
        vec3 normalizedNorm = normalize(rawNormal);
        vec3 lightDir = normalize(lightPos - fragPos);

        float diff = max(dot(lightDir, normalizedNorm), 0.0f);
        diffuseVertex = diff * lightColor;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, normalizedNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specularVertex = specularStrength * spec * lightColor;
    } else {
        ambientVertex = lightColor;
        diffuseVertex = lightColor;
        specularVertex = lightColor;
    }

	gl_Position = projection * view * model * vec4(rawPos.xyz, 1.0);
}