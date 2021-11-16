#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_Color;

out vec4 g_Color;

void main()
{
	g_Color = vec4(a_Color, a_Color, a_Color, 1.0);
	gl_Position = vec4(a_Position, 1.0);
}
