#version 330 core

in vec3 acolor;
in vec2 atexCord;

out vec4 FragColor;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main() {
    vec4 value = (texture(Texture1, atexCord) + texture(Texture2, atexCord)) * 0.5;
    FragColor = (value + vec4(acolor.xyz, 1.0)) / 2.0;

}
