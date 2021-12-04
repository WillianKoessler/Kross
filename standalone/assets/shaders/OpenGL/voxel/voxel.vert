#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

//layout(location = 0) in uvec2 a_Data;

out vec4 v_Color;
out uint v_uVisibleFaces;


void main(void)
{
	vec3 pos = a_Position;
	vec4 color = a_Color;

//	uint F = a_Data.x;
//	uint S = a_Data.y;
//
//	F = 0x000000FFu & a_Data.x;
//	S = 0xFFFFFF00u & a_Data.y;
//
//	pos.x = float((F & 0xFF000000u) >> 24u);
//	pos.y = float((F & 0x00FF0000u) >> 16u);
//	pos.z = float((F & 0x0000FF00u) >> 8u);
//
//	color.r = float(F & 0x000000FFu) / 256.0;
//	color.g = float((S & 0xFF000000u) >> 24u) / 256.0;
//	color.b = float((S & 0x00FF0000u) >> 16u) / 256.0;
//	//color.a = float((S & 0x0000FF00u) >> 8) / 256.0;
//	color.a = 1.0;
//
//	v_uVisibleFaces = S & 0x000000FFu;
////	v_uVisibleFaces |= 0x20u | 0x10u;
////	v_uVisibleFaces = 0xFFu;

	v_Color = color;
	gl_Position = vec4(pos, 1.0);
}











	
//	if(((a_Data.y & 0xFFFFFF00) >> (8 * 3)) != 0x00)
//		v_Color = vec4(1.0, 0.0, 0.0, 1.0);
//	else
//		v_Color = vec4(vec3(0.0), 1.0);

//	v_Color = vec4(0.0);
//	v_Color.r = float((a_Data.x & 0x000000FF) >> (8 * 0)) / 256.0;
//	v_Color.g = float((a_Data.y & 0xFF000000) >> (8 * 3)) / 256.0;
//	v_Color.b = float((a_Data.y & 0x00FF0000) >> (8 * 2)) / 256.0;
//	v_Color.a = 1.0;

//	gl_Position = vec4(vec3(0.0), 1.0);


	//v_Color = a_Color;
	//gl_Position = vec4(a_Position.xyz, 1.0);
