#ifndef SHADER_CPP
#define SHADER_CPP
#include "all.hpp"


    char* Shader::loadFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate); 
    if (!file) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1]; // +1 for null terminator
    if (file.read(buffer, size)) {
        buffer[size] = '\0'; // null-terminate
        return buffer;
    }

    delete[] buffer;
    return nullptr;
}


unsigned int Shader::compileShaders(std::string vertexSource, std::string fragmentSource, std::vector<std::string> locations){
     char* vertexShaderSource = loadFile(vertexSource.c_str());
     char* fragmentShaderSource = loadFile(fragmentSource.c_str());
     unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
    int location_index = 0;
    for(auto location : locations){
        glBindAttribLocation(shaderProgram, location_index, location.c_str());
        location_index++;
    }
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
     return shaderProgram;
}

#endif