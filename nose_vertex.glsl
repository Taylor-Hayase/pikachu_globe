#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec3 vertNorm;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec3 vertex_color;
out vec3 vertex_norm;
out vec4 vertex_pos;

void main()
{
	vertex_color = vertColor;
    vertex_norm = vec4(M * vec4(vertNorm, 0.0)).xyz;
    vertex_pos =  P * V * M * vec4(vertPos, 1.0);
	gl_Position = P * V * M * vec4(vertPos, 1.0);
}
