#version 330 core
out vec4 color;
in vec3 fragTex;
in vec3 fragNor;
in vec4 fragPos;


void main()
{
    vec3 normal = normalize(fragNor);
	//color.rgb = normal;
    color.rgb = vec3(.8, 0.07, 0);//vec3(max(abs(normal.z + 0.2), .07) , max(abs(normal.z + 0.2), 0.07), 0.0)
	color.a=1;	//transparency: 1 .. 100% NOT transparent
}
