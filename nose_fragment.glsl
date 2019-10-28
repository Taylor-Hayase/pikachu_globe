#version 330 core
out vec4 color;
in vec3 vertex_color;
in vec3 vertex_norm;
in vec4 vertex_pos;

void main()
{
    vec3 normal = normalize(vertex_norm);
    vec3 lightpos = vec3(0, 10, 60);
    vec3 ld = normalize(lightpos - vertex_pos.xyz);
    float k_d = dot(normal, ld);
    k_d = clamp(k_d, 0, 1);
    
    float k_a = 0.3;
    color.rgb = k_a * vec3(0.1, 0.1, 0.1) + k_d * vec3(0.1, 0.1, 0.1);
	color.a=1;	//transparency: 1 .. 100% NOT transparent
}
