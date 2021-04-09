#include "include/Shader.h"

void Shader::loadFromFile(const char *path, GLuint shaderType) {
    std::ifstream src(path, std::ifstream::binary);
    if(src){
        src.seekg (0, src.end);
        int length = src.tellg();
        src.seekg (0, src.beg);

        shaderSource = new char [length];
        src.read (shaderSource ,length);
        src.close();

        shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderSource, NULL);
        glCompileShader(shaderID);
    }
}

ShaderProgram::ShaderProgram(Shader *vertex, Shader *fragment) {
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex->shaderID);
    glAttachShader(shaderProgram, fragment->shaderID);
    glLinkProgram(shaderProgram);
    delete vertex;
    delete fragment;
}
