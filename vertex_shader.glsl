#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

void main(){
	vec4 new_coord = transform * vec4(aPos.xyz, 1.0f);
	gl_Position = new_coord;
	ourColor = vec4(inColor.xyz, 1.0);
	TexCoord = aTexCoord;
}
