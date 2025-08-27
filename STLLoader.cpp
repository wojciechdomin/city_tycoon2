#include "all.hpp"

float* STLLoader::loadModel(std::string filename, int &byteCount){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    byteCount = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[byteCount];
    file.read(buffer, byteCount);
    unsigned int triangleCount = *((unsigned int*)(buffer+80));

    float* result = new float[triangleCount*24];

    int j = 0;
    for(int i = 84; i < byteCount; i+=14){
        result[j+0+0] = *((float*)(buffer+i+12+0));
        result[j+0+1] = *((float*)(buffer+i+12+4));
        result[j+0+2] = *((float*)(buffer+i+12+8));
        i+=12; j+=8;
        result[j+0+0] = *((float*)(buffer+i+12+0));
        result[j+0+1] = *((float*)(buffer+i+12+4));
        result[j+0+2] = *((float*)(buffer+i+12+8));
        i+=12; j+=8;
        result[j+0+0] = *((float*)(buffer+i+12+0));
        result[j+0+1] = *((float*)(buffer+i+12+4));
        result[j+0+2] = *((float*)(buffer+i+12+8));
        i+=12; j+=8;

    }

    byteCount = triangleCount;
    delete[] buffer;
    return result;

}