#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

in vec4 v_Color[];
in uint v_uVisibleFaces[];

out vec4 g_Color;

const int X = 3;
const int Y = 5;
const int Z = 4;

void makeQuad(vec4 offset, int axis)
{
	float light = abs(axis) / 5.0;
	vec4 alignment[4];
			if(axis ==  X) { // towards the +X axis
		alignment[0] = vec4( 1.0, 1.0, 1.0, 0.0);
		alignment[1] = vec4( 1.0,-1.0, 1.0, 0.0);
		alignment[2] = vec4( 1.0, 1.0,-1.0, 0.0);
		alignment[3] = vec4( 1.0,-1.0,-1.0, 0.0);
	} else	if(axis ==  Y) { // towards the +Y axis
		alignment[0] = vec4( 1.0, 1.0, 1.0, 0.0);
		alignment[1] = vec4( 1.0, 1.0,-1.0, 0.0);
		alignment[2] = vec4(-1.0, 1.0, 1.0, 0.0);
		alignment[3] = vec4(-1.0, 1.0,-1.0, 0.0);
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
		light = 0.4;
		alignment[0] = vec4(-1.0,-1.0, 1.0, 0.0);
		alignment[1] = vec4(-1.0,-1.0,-1.0, 0.0);
		alignment[2] = vec4( 1.0,-1.0, 1.0, 0.0);
		alignment[3] = vec4( 1.0,-1.0,-1.0, 0.0);
	} else	if(axis == -Z) { // towards the -Z axis
		alignment[0] = vec4( 1.0, 1.0,-1.0, 0.0);
		alignment[1] = vec4( 1.0,-1.0,-1.0, 0.0);
		alignment[2] = vec4(-1.0, 1.0,-1.0, 0.0);
		alignment[3] = vec4(-1.0,-1.0,-1.0, 0.0);
	}
	for(int i = 0; i < 4; i++) {
		g_Color = v_Color[0] * light;
		g_Color.a = v_Color[0].a;
		gl_Position = u_ViewProjection * u_Transform * (offset + alignment[i]);
		EmitVertex();
	}
	EndPrimitive();
}

void cube(vec4 center)
{
	if((v_uVisibleFaces[0] & 0x20) != 0) makeQuad(center,-Y);
	if((v_uVisibleFaces[0] & 0x10) != 0) makeQuad(center, Y);
	if((v_uVisibleFaces[0] & 0x08)  != 0) makeQuad(center, X);
	if((v_uVisibleFaces[0] & 0x04)  != 0) makeQuad(center,-Z);
	if((v_uVisibleFaces[0] & 0x02)  != 0) makeQuad(center,-X);
	if((v_uVisibleFaces[0] & 0x01)  != 0) makeQuad(center, Z);
}

void main(void)
{
	cube(gl_in[0].gl_Position);
}
