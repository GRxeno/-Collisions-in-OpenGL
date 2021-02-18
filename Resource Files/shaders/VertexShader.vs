#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;


out vec2 UV;
out vec3 FragPos;
out vec3 Normal;


uniform mat4 u_V;
uniform mat4 u_M;
uniform mat4 u_P;


void main()
{
    gl_Position =  u_P * u_V  * u_M * vec4(vertexPosition,1);
    UV = vertexUV;
	
	FragPos = vec3(u_M * vec4(vertexPosition, 1.0));
    Normal = mat3(transpose(inverse(u_M))) * vertexNormal;
}

