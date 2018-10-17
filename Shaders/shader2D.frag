#version 430 core

#define PI 3.14159265359

in vec2 vTexCoord0; // texCoords for wall
uniform float scl;	// so tex will need wrap by this value
uniform int width;
uniform int height;

layout(binding = 0) uniform sampler2D tex;
layout(binding = 1) uniform sampler2D shadowMap;

out vec4 fragColor;

uniform float near;
uniform float far;

float shadowBlur = 3; // counted in pixels
float biasPixels = 1. / far * shadowBlur;

float bias = 0.005;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


float spaceToAngle(float x, float y) {

	float result = 0;

	if(x == 0 && y < 0){
		result = PI;
	}
	else if(x > 0){
		result = atan(y/x);
	}
	else{
		result = atan(y/x) + PI;
	}

	return result;
}

float angleToRange(float angle) {

	// out range is [ 0 ; 1 ]
	// angle in is [ 0 ; 2*pi ]
	// we want to apply tan(angle) on [-pi/4; pi/4]
	// 0 beeing horizontal or vertical
	// aka angle 0, pi/2, pi, 3*pi/2
	// |------|------|------|------|
	// 0    pi/2    pi    3*pi/2   2*pi
	float posToFace = mod(angle + PI/4, PI/2) - PI/4; // angle relative to the center of cube proj face
	float shift = angle - posToFace; // the center of said face in texture (aka vertical or horizontal)

	return ((tan(posToFace) * PI/4)  + shift + PI/4)/ 2 / PI; // + PI/4 because texture start at -PI/4 rads angle.
}

void main() {
	fragColor = texture(tex, vTexCoord0);

	// gl_Fragcoord range is [0, screenSize[ . We need to substract player position (middle of screen)
	float angle = spaceToAngle(gl_FragCoord.x - width/2, gl_FragCoord.y - height/2);
	float x = angleToRange(angle);
	float depth = texture(shadowMap, vec2(x, 0)).r;
	float shadowDepth = min(depth / cos(mod(angle + PI/4, PI/2) - PI/4) , 1); // transform square view to circle

	float currentDepth = distance(gl_FragCoord.xy, vec2(width/2, height/2)) / far; // normalized


	if(currentDepth > shadowDepth - biasPixels - bias)
		fragColor *= clamp( (shadowDepth - currentDepth) / biasPixels, 0.05, 1);
	
	fragColor *= 1. - currentDepth;
	
}