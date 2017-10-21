#version 330

in vec2 texCoord;

uniform sampler2D tex; // input texture loaded by BindTexture

out vec4 fragColor;

void main(){

	fragColor = texture(tex, texCoord);
	
}