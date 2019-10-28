#version 330 core
out vec4 color;
in vec3 vertex_color;
in vec3 vertex_norm;
in vec4 fragPos;

void main()
{
    vec3 normal = normalize(vertex_norm);
    vec3 lightpos = vec3(0, 10, 60);
    vec3 ld = normalize(lightpos - fragPos.xyz);
    float k_d = dot(normal, ld);
    k_d = clamp(k_d, 0, 1);
    
    float k_a = 0.7;
    
    color.rgb = k_a * vertex_color + k_d * vertex_color;//vec3(abs(vertex_norm.x) + vertex_color.x, abs(vertex_norm.x) + vertex_color.y, 0.0);
	color.a=1;	//transparency: 1 .. 100% NOT transparent
}
