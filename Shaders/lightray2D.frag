# version 330

#define PI 3.14159265359

//inputs from vertex shader
in vec2 vTexCoord0;

//uniform values
uniform sampler2D u_texture;
uniform vec2 resolution;

//layout(location = 0) out vec3 outColor;
out vec4 screenColor;

//alpha threshold for our occlusion map
const float THRESHOLD = 0.75;

void main(void) {
	float color = 1.0;
	float angle = vTexCoord0.x * 2. * PI;
	float step = 0.5/255.; //resolution.y; max resolution since it's encoded on 8-bits now. Must use r, g, and b as an unsigned 3 byte value to have better quality
  
	for(float dist = 0.; dist < 0.5; dist += step){
		vec2 coord = vec2(cos(angle) * dist + 0.5, sin(angle) * dist + 0.5);
		vec4 data = texture(u_texture, coord);
	  
		if(data.r > 0.5){
			color = dist;
			break;
		}
	}

	
	screenColor = vec4(color);
}