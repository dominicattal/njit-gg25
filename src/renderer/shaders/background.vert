#version 460 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in float aTexId;

layout (std140, binding = 1) uniform View
{
    vec2 offset;
    float scale;
    float aspect_ratio;
};

out vec2 UV;
out flat int ID;

void main() {
    gl_Position = vec4(aPosition, 0.0f, 1.0f);
    UV = aUV;
    UV.x = (UV.x + offset.x * scale) / aspect_ratio;
    ID = int(round(aTexId));
}