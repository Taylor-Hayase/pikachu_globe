#version 330 core
out vec4 color;
in vec3 fragTex;
in vec3 fragNor;
in vec4 fragPos;


void main()
{
	vec3 normal = normalize(fragNor);
    
    if (fragPos.y > 0) {
        color.rgb = vec3(1, 0, 0);
    }
    else {
        color.rgb = vec3(1, 1 ,1);
    }
    
    
	color.a=0.4;	//transparency: 1 .. 100% NOT transparent
}
