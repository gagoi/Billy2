#version 430 core

in vec2 vTexCoord0;

layout(binding = 0) uniform sampler2D tex;

out vec4 color;

uniform float near;
uniform float far;

float width = 1600;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(void) {
  //color = vec4(1.0, 1.0, 1.0, 1.0);

  float depth = LinearizeDepth(gl_FragCoord.z);
  color = vec4(vec3(depth / far), 1.0);

  //color = vec4(0.1);
  // just for debugging tho, no use
}