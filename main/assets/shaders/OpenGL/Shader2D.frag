#version 440 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 color_;

uniform sampler2D u_Texture;
uniform float u_Repeat;
uniform vec4 u_Color;

uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_texCoord;
in float v_TexIndex;

const float offset = 1.0 / pow(2,100);  

void main()
{
//   vec2 offsets[9] = vec2[](
//        vec2(-offset,  offset), // top-left
//        vec2( 0.0f,    offset), // top-center
//        vec2( offset,  offset), // top-right
//        vec2(-offset,  0.0f),   // center-left
//        vec2( 0.0f,    0.0f),   // center-center
//        vec2( offset,  0.0f),   // center-right
//        vec2(-offset, -offset), // bottom-left
//        vec2( 0.0f,   -offset), // bottom-center
//        vec2( offset, -offset)  // bottom-right    
//    );
//    float kernel[9] = float[](
//    -1,-1,-1,
//    -1,9,-1,
//    -1,-1,-1
//    );
//    vec3 sampleTex[9];
//    for(int i = 0; i < 9; i++)
//    {
//        sampleTex[i] = vec3((texture(u_Textures[int(v_TexIndex)], v_texCoord + offsets[i])));
//    }
//    vec3 col = vec3(0.0);
//    for(int i = 0; i < 9; i++)
//        col += sampleTex[i] * kernel[i];
//    
//    color = vec4(col, 1.0);



	// Debug Texture IDs
	//color = vec4(v_texID, v_texCoord, 1.0);

	// Debug Texture Coords
	//color = vec4(v_texCoord, 0.0, 1.0);

	// Solid Color (vertex)
	//color = v_Color;

	// Solid Color (uniform)
	//color = u_Color;

	// Texture + Tint
	//color = texture(u_Texture, v_texCoord * u_Repeat) * u_Color;

	// Texture + Tint (batch)
	color = texture(u_Textures[int(v_TexIndex)], v_texCoord) * v_Color;
	
	
	//color_ = texture(u_Textures[int(v_TexIndex)], v_texCoord) * v_Color;

}
