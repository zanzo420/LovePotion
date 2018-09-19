#include "common/runtime.h"
#include "objects/shader/shader.h"

#include "s_vtx_glsl.h"
#include "s_frag_glsl.h"

Shader::Shader()
{
    char vertexShaderSource[s_vtx_glsl_size + 1];
    memcpy(vertexShaderSource, s_vtx_glsl, s_vtx_glsl_size);
    vertexShaderSource[s_vtx_glsl_size] = '\0';

    this->vertexShader = this->Compile(GL_VERTEX_SHADER, vertexShaderSource);

    char fragmentShaderSource[s_frag_glsl_size + 1];
    memcpy(vertexShaderSource, s_frag_glsl, s_frag_glsl_size);
    vertexShaderSource[s_frag_glsl_size] = '\0';

    this->fragmentShader = this->Compile(GL_FRAGMENT_SHADER, fragmentShaderSource);

    this->CreateProgram();
}

uint Shader::Compile(GLuint type, const char * source)
{
    uint shaderHandle = glCreateShader(type);
    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);

    return shaderHandle;
}

void Shader::CreateProgram()
{
    this->program = glCreateProgram();

    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->fragmentShader);

    glLinkProgram(this->program);

    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
}

uint Shader::GetProgram()
{
    return this->program;
}