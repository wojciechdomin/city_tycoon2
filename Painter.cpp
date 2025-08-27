#include "all.hpp"


class Painter{
    int index;
    float* vertices;

    public:
    void draw_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
};

Painter::Painter(){
    index = 0;
}

void Painter::draw_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    vertices[index+0] = a.x;
    vertices[index+1] = a.y;
    vertices[index+2] = a.z;

    vertices[index+6] = 0.0f;
    vertices[index+7] = 0.0f;
    

    index+=8;
}