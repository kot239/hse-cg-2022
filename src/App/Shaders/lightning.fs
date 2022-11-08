#version 330 core

in vec3 rawPos;
in vec3 fragPos;
in vec3 rawNormal;

in vec3 ambientVertex;
in vec3 diffuseVertex;
in vec3 specularVertex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool isVertexLightning;

out vec4 out_col;

void main()
{
    // color vectors
    vec3 objColor = mix(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), (rawPos.y + 0.22) / 0.4);

    vec3 ambient, diffuse, specular;

    if (isVertexLightning) {
        ambient = ambientVertex;
        diffuse = diffuseVertex;
        specular = specularVertex;
    } else {
        // ambient
        float ambientStrength = 0.1;
        ambient = ambientStrength * lightColor;

        // diffuse
        vec3 normalizedNorm = normalize(rawNormal);
        vec3 lightDir = normalize(lightPos - fragPos);

        float diff = max(dot(lightDir, normalizedNorm), 0.0f);
        diffuse = diff * lightColor;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(- fragPos);
        vec3 reflectDir = reflect(-lightDir, normalizedNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = specularStrength * spec * lightColor;
    }

    vec3 result = (ambient + diffuse + specular) * objColor;

    out_col = vec4(result.xyz, 1.0f);
}