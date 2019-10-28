#version 330 core
out vec4 color;
in vec3 fragTex;
in vec3 fragNor;
in vec4 fragPos;


void main()
{
    vec3 normal = normalize(fragNor);
    vec3 lightpos = vec3(0, 10, 60);
    vec3 ld = normalize(lightpos - fragPos.xyz);
    float k_d = dot(normal, ld);
    k_d = clamp(k_d, 0, 1);
    
    float k_a = 0.3;
	//color.rgb = normal;
    color.rgb = k_a * vec3(1, 0.27, 0) + k_d * vec3(1, 0.27, 0);
	color.a=1;	//transparency: 1 .. 100% NOT transparent
}
