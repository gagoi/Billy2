#version 430 core

#define PI 3.14159265359

in vec2 vTexCoord0;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D shadowMap;

uniform vec2 resolution;

out vec4 fragColor;

void main(){
	fragColor = texture(tex, vTexCoord0);
	
	vec2 rectCoord = vTexCoord0 - 0.5;
	float angle = atan(rectCoord.y/rectCoord.x);
	float dist = sqrt(pow(rectCoord.y, 2) + pow(rectCoord.x, 2));
	if(rectCoord.x < 0.)
		angle += PI;
	
	angle /= 2.*PI;
	
	while(angle > 1.)
		angle -= 1.;
	while(angle < 0.)
		angle += 1.;
	
	// lightmap interpolation with float angle
	float pixelPos = angle * resolution.x; 	// pixel position in lightmap
	float pixelBase1 = floor(pixelPos); 	// immediate left pixel on lightmap
	float pixelBase2 = pixelBase1 + 1.; 	// immediate right pixel on lightmap
	float percent1 = pixelBase2 - pixelPos; // percentage of interpolation on the first pixel (on 0 to 1 scale)
											
											/* | - - - - - - * - - - |
										pixel 1			   value	pixel 2
												<----------->
												 percentage 1 = pixel 2 - value
											*/

	float percent2 = 1. - percent1; 		// percentage of interpolation on the second pixel
	
	
	float lightDistance = texture(shadowMap, vec2(pixelBase1 / resolution.x, 0)).r * percent1 + texture(shadowMap, vec2(pixelBase2 / resolution.x, 0)).r * percent2;
	
	if(dist > lightDistance)
		fragColor *= 0.2; // for debug use, we only fade the display
		
}