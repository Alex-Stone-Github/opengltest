#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // lib dir
#include "shader.h"

#define FPS 30
#define DT 1.0f / (float)FPS


bool running = true;

float vertices[] = {
    0.5f,  0.5f, 0.0f,       1.0f, 0.0f, 0.0f,  0.0f, 0.0f,// top right
    0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,      0.0f, 0.0f, 1.0f,  0.0f, 1.0f    // top left 
};  
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
};

unsigned long counter = 0;

int main() {
    // setup
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* window = SDL_CreateWindow(
            "Opengl Yay",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            800,
            600,
            SDL_WINDOW_OPENGL
            );
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    glewInit();

    // opengl stuff
    // vao stup and binding
    unsigned int vao;
    glGenVertexArrays(1, &vao);

    // buffers
    glBindVertexArray(vao); // make sure that all of this is being applied to the vao
    unsigned int vbo; // vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // This makes this object current
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // this fills the buffer with vertex data from vertices
    unsigned int ebo; // index buffer object
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // fancy vao stuff // vertex attrib pointer / seting up stuff for shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // shaders
    unsigned int shaderProgram = createShaderProgram("./res/vertex.glsl", "./res/fragment.glsl");


    // textures and stuff
    // settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    // mipmaps are a cool way
    // to down res an image
    // as you get far away from
    // it, but be away of this line and be ready to switch it to linear because it seems suspicious to me
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //loading
    //scoping
    unsigned int texture1;
    unsigned int texture2;
    {
        int width;
        int height;
        int nrChannels;
        unsigned char* data = stbi_load("./res/texture2.png", &width, &height, &nrChannels, 0);
        if (!data) {
            // panic
            printf("Couldn't load texture\n");
            return 0;
        }
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }    
    {
        int width;
        int height;
        int nrChannels;
        unsigned char* data = stbi_load("./res/texture.jpg", &width, &height, &nrChannels, 0);
        if (!data) {
            // panic
            printf("Couldn't load texture\n");
            return 0;
        }
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    
    // other settings
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // fill
    // loop
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        // set uniforms
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "Texture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "Texture2"), 1);

        // background
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // textures
        glActiveTexture(GL_TEXTURE0); // texture unit zero
        glBindTexture(GL_TEXTURE_2D, texture2);//this automatically assigns the sampler 2d uniform in the frag shader
        glActiveTexture(GL_TEXTURE1); // texture unit one
        glBindTexture(GL_TEXTURE_2D, texture1);//this automatically assigns the sampler 2d uniform in the frag shader

        // draw 
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // update 
        SDL_GL_SwapWindow(window);
        counter ++;
        sleep(DT);
    }

    // cleanup
    glDeleteShader(shaderProgram);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

