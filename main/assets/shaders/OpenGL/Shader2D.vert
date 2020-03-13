#version 440 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in vec2  a_TexFlip;
layout(location = 4) in float a_TexIndex;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_texCoord;
out float v_TexIndex;


void main()
{
	v_Color = a_Color;
	v_texCoord = a_TexCoord;

	// mirror effect
	

	if(float(a_TexFlip.x) > 0.0)
	{
		v_texCoord.x = 1.0 - v_texCoord.x;
	}
	//if(float(a_TexFlip.y) > 0.0)
	//{
	//	v_texCoord.y = 1.0 - v_texCoord.y;
	//}

	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
