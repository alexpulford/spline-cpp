#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

out vec3 fColor;

uniform mat4 matrix;
uniform float ratio;

in VS_OUT {
    vec3 color;
    float width;
} gs_in[];

const float WIDTH = 0.0015;

void emit_rect(vec4 p1, vec4 p2, vec3 c1, vec3 c2, vec4 u) {
    gl_Position = matrix * (p1 + u); fColor = c1; EmitVertex();
    gl_Position = matrix * (p2 + u); fColor = c2; EmitVertex();
    gl_Position = matrix * (p1 - u); fColor = c1; EmitVertex();
    gl_Position = matrix * (p2 - u); fColor = c2; EmitVertex();
    EndPrimitive();
}

void main() {
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec3 c1 = gs_in[0].color;
    vec3 c2 = gs_in[1].color;

    float w = 1.0;//gs_in[0].width;//(gs_in[0].width + gs_in[1].width)/2.0;
    vec4 hor = vec4(cross(vec3(p1.xy,0), vec3(p2.xy,0)), 0);
    vec4 up = vec4(normalize(cross(hor.xyz, vec3((p2 - p1).xy, 0))), 0);
    if(up.y != 0.0f) {
        up.y = (up.y/abs(up.y)) * ratio;
    }


    emit_rect(p1, p2, c1, c2,  WIDTH * up);
}
