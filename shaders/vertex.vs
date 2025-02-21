#version 330 core
uniform mat4 MVP;
uniform float offset;
in vec3 vCol;
in vec2 vPos;
out vec3 color;
void main() {
    gl_Position = MVP * vec4(vPos.x + offset, vPos.y, 0.0, 1.0);
    color = vCol;
}
