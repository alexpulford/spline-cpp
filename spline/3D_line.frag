#version 330 core
in vec3 fColor;
in vec3 fPos;
in vec3 fNormal;
out vec4 outColor;

const vec3 light = vec3(10, 0, 20);
const vec3 light2 = vec3(20, 10, 0);
vec3 get_color(vec3 base, vec3 normal, vec3 pos) {
    float f = abs(dot(normal, normalize(light - pos))) + abs(dot(normal, normalize(light2 - pos)));
    return base*(max(f/1.2, 0.3));
}

void main() {
    outColor = vec4(get_color(fColor, fNormal, fPos), 1.0);
}
