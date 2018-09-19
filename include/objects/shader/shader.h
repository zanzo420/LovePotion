#pragma once

class Shader
{
    public:
        Shader();
        uint GetProgram();

    private:
        uint Compile(GLenum type, const char * source);
        void CreateProgram();

        uint program;
        uint vertexShader;
        uint fragmentShader;
};