#version 440 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_texCoord;
in float v_TexIndex;

void main()
{
	// Texture + Tint (batch)
	int index = int(v_TexIndex);
	if(index == 0){
		color = v_Color;
	} else if(index > 0){
		color = texture(u_Textures[index], v_texCoord) * v_Color;
	}
}
