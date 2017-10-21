#version 330 compatibility

in vec2 position;
in vec2 texCoord;

out vec2 texCoordOut;

uniform mat4 projection;
uniform mat4 modelview;

void main(){
	gl_Position =  projection * modelview * vec4(position, 0, 1.0);
	texCoordOut = texCoord;
}