# version 330 core

#define PI 3.14159265359

//inputs from vertex shader
in vec2 vTexCoord0;

//uniform values
uniform sampler2D u_texture;
uniform vec2 resolution;

//layout(location = 0) out vec3 outColor;
out float shadow;

//alpha threshold for our occlusion map
const float THRESHOLD = 0.75;

void main(void) {
	float color = 0.0;
	float angle = vTexCoord0.x * 2. * PI;
	float angleCos = cos(angle);
	float angleSin = sin(angle);
	float step = 0.5/resolution.y*5;
	
	float dist;
	vec4 data;
	bool broadPhasePassed = false;
	
	if(texture(u_texture, vec2(0.5,0.5)).r < 0.5){
		color = 1.;
		
		for(dist = 0.; dist < 0.5; dist += step){
			vec2 coord = vec2(angleCos * dist + 0.5, angleSin * dist + 0.5);
			data = texture(u_texture, coord);
		  
			if(data != vec4(0)){
				broadPhasePassed = true;;
				break;
			}
		}
		
		if(broadPhasePassed){
			step /= 5;
			
			while(data != vec4(0)){
				dist -= step;
				vec2 coord = vec2(angleCos * dist + 0.5, angleSin * dist + 0.5);
				data = texture(u_texture, coord);
			}
			
			float maxdist = dist + 0.01;
			
			color = maxdist;
		}
	}
	
	shadow = color;
}