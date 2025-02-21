#version 330 core
uniform float uniColor;
in vec3 color;
out vec4 fragment;
void main() {
    fragment = vec4(uniColor * color, 1.0);
}
