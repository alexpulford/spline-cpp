#version 330 core
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 48) out;
out vec3 fColor;
out vec3 fPos;
out vec3 fNormal;

uniform mat4 matrix;

in VS_OUT {
    vec3 color;
} gs_in[];

const float WIDTH = 0.11;

void emit_bar(vec4 p1, vec4 u1, vec4 h1, vec3 c1, vec4 p2, vec4 u2, vec4 h2, vec3 c2) {
    for(int i = 0; i <= 1; i++) {
        for(int s = -1; s <= 1; s+=2) {
            vec3 norm = normalize((s * h1).xyz + (s * h2).xyz);

            fColor = vec3(1,1,1);
            fPos = p1.xyz;
            fNormal = norm;
            gl_Position = matrix * (p1 + u1 + s * h1); EmitVertex();
            gl_Position = matrix * (p1 - u1 + s * h1); EmitVertex();
            gl_Position = matrix * (p2 - u2 + s * h2); EmitVertex();
            EndPrimitive();

            fColor = vec3(1,1,1);
            fPos = p2.xyz;
            fNormal = norm;
            gl_Position = matrix * (p1 + u1 + s * h1); EmitVertex();
            gl_Position = matrix * (p2 + u2 + s * h2); EmitVertex();
            gl_Position = matrix * (p2 - u2 + s * h2); EmitVertex();
            EndPrimitive();
        }
        vec4 tmp = h1;
        h1 = u1;
        u1 = tmp;
        tmp = h2;
        h2 = u2;
        u2 = tmp;
    }

}

void main() {

    vec3 c0 = gs_in[0].color;
    vec3 c1 = gs_in[1].color;
    vec3 c2 = gs_in[2].color;

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;

    vec4 up = vec4(WIDTH * normalize(cross(p1.xyz, p2.xyz)), 0);
    vec4 hor = vec4(WIDTH * normalize(cross(up.xyz, (p2 - p1).xyz)), 0);

    emit_bar(p1, up, hor, c1, p2, up, hor, c2);

    vec4 up_c = vec4(WIDTH * normalize(cross(p0.xyz, p1.xyz)), 0);
    vec4 hor_c = vec4(WIDTH * normalize(cross(up_c.xyz, (p1 - p0).xyz)), 0);

    emit_bar(p1, up, hor, c1, p1, up_c, hor_c, c1);

}
