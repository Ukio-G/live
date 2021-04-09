//
// Created by ukio on 30.06.2020.
//

#ifndef LIVE_SHADER_H
#define LIVE_SHADER_H

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "SFML/OpenGL.hpp"

class Shader {
public:
    char * shaderSource = nullptr;
    GLuint shaderID;
    void loadFromFile(const char * path, GLuint shaderType);
    Shader(const char * path, GLuint shaderType) {
        loadFromFile(path, shaderType);
    }
    ~Shader() {
        glDeleteShader(shaderID);
        delete shaderSource;
    }
};

class  ShaderProgram {
public:
    GLuint shaderProgram;
    ShaderProgram(Shader * vertex, Shader * fragment);

    void use() {
        glUseProgram(shaderProgram);
    }
};


#endif //LIVE_SHADER_H
