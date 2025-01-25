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
    vec2 position = scale * (aPosition - offset);
    position.y /= aspect_ratio;
    gl_Position = vec4(position, 0.0f, 1.0f);
    UV = aUV;
    ID = int(round(aTexId));
}