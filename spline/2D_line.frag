#version 330 core
in vec3 fColor;
in vec3 fPos;
out vec4 outColor;

void main() {
    outColor = vec4(fColor, 1.0);
}
