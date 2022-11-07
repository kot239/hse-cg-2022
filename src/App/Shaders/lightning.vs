#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform int time;

out vec3 rawPos;
out vec3 fragPos;
out vec3 rawNormal;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    int real_time = 0;
    if (time >= 300 * 5) {
        real_time = time;
    }
    float beta = 1.5 * M_PI * (pos.y + 0.22) / 0.4;
    vec3 endPos = vec3(pos.x * cos(beta) - pos.z * sin(beta), pos.y, pos.x * sin(beta) + pos.z * cos(beta));
    rawPos = mix(pos.xyz, endPos, sin(2 * M_PI * (real_time % 300) / 300.0));
	fragPos = vec3(view * model * vec4(rawPos.xyz, 1.0));
	vec3 endNorm = vec3(norm.x * cos(beta) - norm.z * sin(beta), norm.y, norm.x * sin(beta) + norm.z * cos(beta));
	rawNormal = normalMatrix * mix(norm.xyz, endNorm, sin(2 * M_PI * (real_time % 300) / 300.0));
	gl_Position = projection * view * model * vec4(rawPos.xyz, 1.0);
}