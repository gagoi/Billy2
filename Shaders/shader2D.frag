#version 330 compatibility

in vec2 texCoordOut;

uniform sampler2D tex;
out vec4 fragColor;

void main(){

	fragColor = texture(tex, texCoordOut);

}