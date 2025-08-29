#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the position variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // the position variable has attribute position 2
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
//uniform mat4 transform;



void main()
{
    /*int i = gl_InstanceID;
    int j = i  % 4;
    i = i / 4;*/
    vec4 bbb = vec4(0.01*aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(aColor,1.0); // set the output variable to a dark-red color
   
    gl_Position =    projection * view * model  * bbb;
}