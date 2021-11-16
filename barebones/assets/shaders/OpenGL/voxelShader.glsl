#type vertex
#version 440 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in float a_Color;

out float g_Color;

void main()
{
	g_Color = a_Color;
	gl_Position = vec4(a_Position, 1.0);
}

#type geometry
#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

in float g_Color[];
out vec4 v_Color;

const int X = 1;
const int Y = 2;
const int Z = 3;

void makeQuad(vec4 offset, int axis)
{
	vec4 alignment[4];
			if(axis ==  X) { // towards the +X axis
		alignment[0] = vec4( 1.0, 1.0, 1.0, 0.0);
		alignment[1] = vec4( 1.0,-1.0, 1.0, 0.0);
		alignment[2] = vec4( 1.0, 1.0,-1.0, 0.0);
		alignment[3] = vec4( 1.0,-1.0,-1.0, 0.0);
	} else	if(axis ==  Y) { // towards the +Y axis
		alignment[0] = vec4(-1.0, 1.0, 1.0, 0.0);
		alignment[1] = vec4(-1.0, 1.0,-1.0, 0.0);
		alignment[2] = vec4( 1.0, 1.0, 1.0, 0.0);
		alignment[3] = vec4( 1.0, 1.0,-1.0, 0.0);
	} else	if(axis ==  Z) { // towards the +Z axis
		alignment[0] = vec4(-1.0, 1.0, 1.0, 0.0);
		alignment[1] = vec4(-1.0,-1.0, 1.0, 0.0);
		alignment[2] = vec4( 1.0, 1.0, 1.0, 0.0);
		alignment[3] = vec4( 1.0,-1.0, 1.0, 0.0);
	} else	if(axis == -X){ // towards the -X axis
		alignment[0] = vec4(-1.0, 1.0,-1.0, 0.0);
		alignment[1] = vec4(-1.0,-1.0,-1.0, 0.0);
		alignment[2] = vec4(-1.0, 1.0, 1.0, 0.0);
		alignment[3] = vec4(-1.0,-1.0, 1.0, 0.0);
	} else	if(axis == -Y) { // towards the -Y axis
		alignment[0] = vec4( 1.0,-1.0, 1.0, 0.0);
		alignment[1] = vec4( 1.0,-1.0,-1.0, 0.0);
		alignment[2] = vec4(-1.0,-1.0, 1.0, 0.0);
		alignment[3] = vec4(-1.0,-1.0,-1.0, 0.0);
	} else	if(axis == -Z) { // towards the -Z axis
		alignment[0] = vec4( 1.0, 1.0,-1.0, 0.0);
		alignment[1] = vec4( 1.0,-1.0,-1.0, 0.0);
		alignment[2] = vec4(-1.0, 1.0,-1.0, 0.0);
		alignment[3] = vec4(-1.0,-1.0,-1.0, 0.0);
	}
	for(int i = 0; i < 4; i++) { v_Color = vec4(g_Color[0], g_Color[0], g_Color[0], 1.0); gl_Position = u_ViewProjection * u_Transform * (offset + alignment[i]); EmitVertex(); }
	EndPrimitive();
}

void main(void)
{
	makeQuad(gl_in[0].gl_Position, Z);
	makeQuad(gl_in[0].gl_Position,-X);
	makeQuad(gl_in[0].gl_Position,-Z);
	makeQuad(gl_in[0].gl_Position, X);
	makeQuad(gl_in[0].gl_Position, Y);
	makeQuad(gl_in[0].gl_Position,-Y);
}

#type fragment
#version 440 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}
