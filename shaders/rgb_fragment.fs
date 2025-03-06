#version 330 core
in vec3 color;  // Unused
in vec2 texture;
out vec4 fragment;

uniform sampler2D outTexture;

void main() {
    fragment = texture(outTexture, texture);
}
