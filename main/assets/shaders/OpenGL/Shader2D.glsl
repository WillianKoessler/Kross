// Basic Texture Shader

#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out float v_texID;
out vec2 v_texCoord;
out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	v_texCoord = a_TexCoord;
	v_texID = a_TexID;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}


#type fragment
#version 440 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform float u_Repeat;
uniform vec4 u_Color;

uniform sampler2D u_Textures[32];

in vec2 v_texCoord;
in vec4 v_Color;

in float v_texID;

void main()
{
	// Debug Texture IDs
	//color = vec4(v_texID, v_texID, v_texID, 1.0);

	// Debug Texture Coords
	//color = vec4(v_texCoord, 0.0, 1.0);

	// Solid Color (vertex)
	//color = v_Color;

	// Solid Color (uniform)
	//color = u_Color;

	// Texture + Tint
	//color = texture(u_Texture, v_texCoord * u_Repeat) * u_Color;

	// Texture + Tint (batch)
	int index = int(v_texID);
	color = texture(u_Textures[index], v_texCoord);
}

