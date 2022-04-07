#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "GL/glew.h"

#include "GLFW/glfw3.h"

// Load a .BMP file using our custom loader
GLuint load_bmp_custom(const std::string &path, int id_texture);
//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like load_bmp_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char *imagepath);


#endif