#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

// out vec3 viewPos;
out vec3 fragPos;
out vec3 rawNormal;

void main()
{
	fragPos = vec3(view * model * vec4(pos.xyz, 1.0));
	// viewPos = vec3(view * model * vec4(pos.xyz, 1.0));
	rawNormal = normalMatrix * norm;
	gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
