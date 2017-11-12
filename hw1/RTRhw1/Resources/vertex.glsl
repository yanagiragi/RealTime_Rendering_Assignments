#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 MVP;
out vec4 ourColor;

void main()
{
	gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0);
	ourColor = vec4(normal.x, normal.y, normal.z, 1.0);
}