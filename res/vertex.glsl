#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCord;

out vec3 acolor;
out vec2 atexCord;

void main() {
    gl_Position = vec4(position.xyz, 1.0);
    acolor = color;
    atexCord = textureCord;
}
