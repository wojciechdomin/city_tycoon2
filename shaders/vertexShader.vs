#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
//uniform mat4 transform;



void main()
{
    int i = gl_InstanceID;
    int j = i  % 33;
    i = i / 33;
    vec4 bbb = vec4(0.1*aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.9*j/10.0, 0.9*i/10.0, 1.0, 1.0); // set the output variable to a dark-red color
    //bbb = vec4(0.0+j*1.0,0.0+i*1.0,9.0f,1.0f)+vec4(aPos, 0.0);
    float factor = 1.0;
    mat4 result;
    for(int i = 0; i < 4; i++)
        result[i] = mix(mat4(1.0)[i], projection[i], factor);

    gl_Position =    /*projection * view * model */ view * bbb;
}