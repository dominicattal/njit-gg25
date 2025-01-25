#version 460

out vec4 FragColor;

uniform sampler2D textures[16];

struct Tex {
    float u;
    float v;
    float w;
    float h;
    int location;
};

layout (std430, binding = 1) readonly buffer Textures
{
    Tex texs[];
};

void main() {
    FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}