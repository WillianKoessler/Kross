// Basic Texture Shader

#type vertex
#version 440 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_texCoord;
out float v_TexIndex;


void main()
{
	v_Color = a_Color;
	v_texCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
#type fragment
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
	// Debug Solid Color
	// color = vec4(1.0, 1.0, 1.0, 1.0);

	// Debug Texture Coords
	//color = vec4(v_texCoord, 0.0, 1.0);

	// Solid Color
	color = vec4(v_Color.rgb, 1.0);

	// Texture + Tint
	//color = texture(u_Texture, v_texCoord * u_Repeat) * v_Color;

	// Texture + Tint (batch)
	// color = texture(u_Textures[int(v_TexIndex)], v_texCoord) * v_Color;
}