#version 460 core

layout (location = 0) in uint aInfo;
layout (location = 1) in uint aInstanceInfo;

layout (std140, binding = 1) uniform Matrices
{
    mat4 view;
    mat4 proj;
};

uniform ivec3 chunk_pos;

#define NEGX 0
#define POSX 1
#define NEGY 2
#define POSY 3
#define NEGZ 4
#define POSZ 5

out vec2 UV;
out flat uint ID;
out flat vec3 normal;

vec3 normals[6] = {
    vec3(-1, 0, 0),
    vec3(1, 0, 0),
    vec3(0, -1, 0),
    vec3(0, 1, 0),
    vec3(0, 0, -1),
    vec3(0, 0, 1)
};

void main() {
    uint width = 1 + ((aInstanceInfo >> 15) & 31);
    uint height = 1 + ((aInstanceInfo >> 20) & 31);
    uint a = width * (aInfo & 1);
    uint b = height * ((aInfo >> 1) & 1);
    uint face = (aInstanceInfo >> 25) & 7;
    normal = normals[face];
    vec3 position;
    switch (face) {
       case NEGX:
            position = vec3(0, a, b);
            UV = vec2(b, width-a);
            break;
        case POSX:
            position = vec3(1, b, a);
            UV = vec2(a, height-b);
            break;
        case NEGY:
            position = vec3(b, 0, a);
            UV = vec2(a, b);
            break;
        case POSY:
            position = vec3(a, 1, b);
            UV = vec2(a, b);
            break;
        case NEGZ:
            position = vec3(a, b, 0);
            UV = vec2(a, height-b);
            break;
        case POSZ:
            position = vec3(b, a, 1);
            UV = vec2(b, width-a);
            break;
    }
    vec3 offset = vec3(aInstanceInfo & 31, (aInstanceInfo >> 5) & 31, (aInstanceInfo >> 10) & 31);
    ID = (aInstanceInfo >> 28);
    gl_Position = proj * view * vec4(position + offset + chunk_pos, 1.0);
}