#version 330 core
layout (location = 0) in vec3 aPosition; // I think this takes in a vec3 from position 0 of a vao 
// vao = vertex array object i think



// VERTEX SHADER
void main() {
    // gl position is a variable glsl expects to be set in this shader, the vertex shader
    gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0); // fancy shading language stuff you can to xyz and it is kind of like the spread operator
}
