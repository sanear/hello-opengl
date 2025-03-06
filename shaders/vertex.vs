#version 330 core
uniform mat4 MVP;
in vec3 vCol;
in vec2 vPos;
in vec2 vTex;
out vec3 color;
out vec2 texture;
void main() {
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
   texture = vTex;
}
