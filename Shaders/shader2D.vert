#version 330 core

in vec2 position;
in vec2 texCoord;

out vec2 vTexCoord0;

uniform mat4 projection;
uniform mat4 modelview;

void main(){
	gl_Position =  projection * modelview * vec4(position, 0, 1);
	vTexCoord0 = texCoord;
}