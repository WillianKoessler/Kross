// Basic Geometry Shader

#type geometry
#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 8) out;

in vec3 baseColor[];
out vec3 color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void makeQuad(vec4 position)
{
    gl_Position = u_ViewProjection * u_Transform * (position + vec4(-1.0, 1.0, 0.0, 0.0););
    color = baseColor[0] * vec3(1.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = u_ViewProjection * u_Transform * (position + vec4(-1.0, -1.0, 0.0, 0.0));
    color = baseColor[0] * vec3(1.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = u_ViewProjection * u_Transform * (position + vec4(1.0, 1.0, 0.0, 0.0));
    color = baseColor[0] * vec3(1.0, 0.0, 0.0);
    EmitVertex();

    gl_Position = u_ViewProjection * u_Transform * (position + vec4(1.0, -1.0, 0.0, 0.0))
    color = baseColor[0] * vec3(1.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

void main(void)
{
    makeQuad(gl_in[0].gl_Position);
}
