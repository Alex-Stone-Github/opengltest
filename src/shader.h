#ifndef SHADER_HHH
#define SHADER_HHH

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

unsigned int createShaderProgram(const char* vshaderpath, const char* fshaderpath);
/*
 * Please note that this is a bad way of reading a
 * file and should only be used for smaller files
 */
static void readFile(char * const buffer, unsigned int buffer_size, const char* filepath);
#endif // SHADER_HHH
