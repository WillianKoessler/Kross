#version 440 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec4 g_Color;

void main()
{
	color = g_Color;
}
