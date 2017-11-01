#version 430 core

#define PI 3.14159265359

in vec2 vTexCoord0;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D shadowMap;

uniform vec2 resolution;

out vec4 fragColor;

void main(){
	fragColor = vec4(0.5) + texture(tex, vTexCoord0);
	
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
	/*
	float pixelPos = angle * resolution.x;
	float pixelBase1 = floor(pixelPos);
	float pixelBase2 = pixelBase1 + 1.;
	float percent1 = pixelPos - pixelBase1;
	float percent2 = 1.-percent1;
	while(pixelBase2 > resolution.x)
		pixelBase2 -= resolution.x;
	
	
	float lightDistance = texture(shadowMap, vec2(pixelBase1 / resolution.x, 0)).r * percent2 + texture(shadowMap, vec2(pixelBase2 / resolution.x, 0)).r * percent1;*/
	float lightDistance = texture(shadowMap, vec2(angle, 0)).r;
	
	if(dist > lightDistance)
		fragColor /= 2.;
	
	//fragColor = vec4(pixelBase1 / resolution.x);
	
}