#version 330 core
out vec4 FragColor;
  
in vec4 gsColor; // the input variable from the vertex shader (same name and same type) 
in vec3 gsNormal; 
in vec3 gsPos;
uniform float time;
void main()
{
    FragColor = vec4( (1.0-gsColor.xyz)*clamp(dot(vec3(0.0,1.0,1.0),gsNormal),0.05,1.0),1.0f);
} 