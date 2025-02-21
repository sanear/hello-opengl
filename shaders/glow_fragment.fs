#version 330 core
uniform float glowFactor;
in vec3 color;
out vec4 fragment;
void main() {
    fragment = vec4(glowFactor * color, 1.0);
}
