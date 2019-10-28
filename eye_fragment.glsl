#version 330 core
out vec4 color;
in vec3 fragTex;
in vec3 fragNor;
in vec4 fragPos;

uniform vec3 camPos;

void main()
{
    vec3 normal = normalize(fragNor);
    vec3 lightpos = vec3(0, 5, 100);
    vec3 ld = normalize(lightpos - fragPos.xyz);
    float k_d = dot(normal, ld);
    k_d = clamp(k_d, 0, 1);
    
    float k_a = 0.3;
    
   /* vec3 cd = normalize(camPos - fragPos.xyz);
    vec3 h = normalize(cd + ld);
    float k_s = dot(h, normal);
    k_s = clamp(k_s, 0, 1);
    k_s = pow(k_s, 1);
*/

    color.rgb = k_a * vec3(0, 0, 0) + k_d * vec3(0, 0, 0);
    
	color.a=1;	//transparency: 1 .. 100% NOT transparent
}
