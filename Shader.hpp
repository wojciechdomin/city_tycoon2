#ifndef SHADER_HPP
#define SHADER_HPP
#include "all.hpp"
class Shader{
private:
    static char* loadFile(const char* filename);
public:
    static unsigned int compileShaders(std::string vertexSource, std::string fragmentSource, std::vector<std::string> locations);
    static unsigned int compileShaders(std::string vertexSource, std::string geometryShader ,std::string fragmentSource, std::vector<std::string> locations);
};
#endif