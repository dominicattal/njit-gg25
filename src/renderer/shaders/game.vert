#version 460 core

layout (location = 0) in vec2 aPosition;

layout (std140, binding = 1) uniform View
{
    vec2 offset;
    float scale;
    float aspect_ratio;
};

void main() {
    vec2 position = scale * (aPosition - offset);
    position.y /= aspect_ratio;
    gl_Position = vec4(position, 0.0f, 1.0f);
}