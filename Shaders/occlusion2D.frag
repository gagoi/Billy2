# version 420 core

in vec2 vTexCoord0;

layout(binding = 0) uniform sampler2D tex;

out vec4 color;

void main(void) {
  color = texture(tex, vTexCoord0);
}