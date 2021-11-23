#version 440 core

layout(location = 0) out vec4 color;

in vec4 g_Color;

void main()
{
	color = g_Color;
	color.a = 1.0;
}
