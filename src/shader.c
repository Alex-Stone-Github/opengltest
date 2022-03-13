#include "shader.h"

unsigned int createShaderProgram(const char* vshaderpath, const char* fshaderpath) {
    // shaders
    const unsigned int buffer_size = 500;
    char* vertex_shader_text_buffer = malloc(buffer_size); // allocate for the file
    char* fragment_shader_text_buffer = malloc(buffer_size); // allocate for the file
    memset(vertex_shader_text_buffer, 0, buffer_size); // fill with null terminations
    readFile(vertex_shader_text_buffer, buffer_size, vshaderpath);
    memset(fragment_shader_text_buffer, 0, buffer_size); // fill with null terminations
    readFile(fragment_shader_text_buffer, buffer_size, fshaderpath);

    printf("%s", vertex_shader_text_buffer);
    printf("%s", fragment_shader_text_buffer);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertex_shader_text_buffer, NULL); // last can be null because shader source code is null terminated
    glShaderSource(fragmentShader, 1, &fragment_shader_text_buffer, NULL); // last can be null because shader source code is null terminated
    // free source code
    free(vertex_shader_text_buffer);
    free(fragment_shader_text_buffer);

    // resuse recycle redo
    int success;
    const int log_length = 512; // half a K
    char log[log_length]; // thats a lot for the stack
    // compile both and check for errors
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, log_length, NULL, log);
        printf("Failed to compile vertex shader + %s", log);
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, log_length, NULL, log);
        printf("Failed to compile fragment shader + %s", log);
    }
   // link those shaders
   unsigned int shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);
   // check
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, log_length, NULL, log);
        printf("Failed to link shader program + %s", log);
    }
    // also delete the shaders now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // return the shader program
    return shaderProgram;
}
static void readFile(char * const buffer, unsigned int buffer_size, const char* filepath) {
    FILE* file = fopen(filepath, "r");
    fread(buffer, 1, buffer_size, file); 
    fclose(file);
}
