#version 440 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform float u_Repeat;
uniform vec4 u_Color;

uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_texCoord;
in float v_TexIndex;

void main()
{
	// Debug Texture Coords
	//color = vec4(v_texCoord, 0.0, 1.0);

	// Solid Color
	color = v_Color;

	// Texture + Tint
	//color = texture(u_Texture, v_texCoord * u_Repeat) * v_Color;

	// Texture + Tint (batch)
	//color = texture(u_Textures[int(v_TexIndex)], v_texCoord) * v_Color;
}