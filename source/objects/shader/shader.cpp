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
    memcpy(fragmentShaderSource, s_frag_glsl, s_frag_glsl_size);
    fragmentShaderSource[s_frag_glsl_size] = '\0';

    this->fragmentShader = this->Compile(GL_FRAGMENT_SHADER, fragmentShaderSource);

    this->CreateProgram();
}

uint Shader::Compile(GLuint type, const char * source)
{
    GLint success;
    GLchar msg[512];

    LOG("Shader Source:\n%s\n", source);
    uint shaderHandle = glCreateShader(type);
    if (!shaderHandle)
    {
        LOG("Cannot create Shader type %u", type);
        return 0;
    }

    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);

    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderHandle, sizeof(msg), nullptr, msg);
        LOG("%u: %s\n", type, msg);
        glDeleteShader(shaderHandle);
        return 0;
    }

    return shaderHandle;
}

void Shader::CreateProgram()
{
    GLint success;

    this->program = glCreateProgram();

    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->fragmentShader);

    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char buffer[512];
        glGetProgramInfoLog(this->program, sizeof(buffer), nullptr, buffer);
        LOG("Link error: %s", buffer);
    }

    glDeleteShader(this->vertexShader);
    glDeleteShader(this->fragmentShader);
}

uint Shader::GetProgram()
{
    return this->program;
}