#version 150
uniform mat4 matrix;
in vec3 position;
in vec3 color;
out vec3 fColor;
void main(){
    fColor = color;
    gl_Position = matrix * vec4(position, 1.0);
}
