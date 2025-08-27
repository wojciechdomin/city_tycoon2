#include <SFML/Window/Window.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "all.hpp"


int main(){
	sf::ContextSettings settings;
     settings.majorVersion = 3;
     settings.minorVersion = 3;
     settings.depthBits = 24;
     settings.stencilBits = 8;
     settings.attributeFlags = sf::ContextSettings::Core;
     
     
     sf::Window window(sf::VideoMode({800,600}), "Okno", sf::Style::Default, settings);
     window.setFramerateLimit(60);
     if(!gladLoadGL()){
          std::cerr<<"Failed to initialize GLAD!\n";
          return -1;
     }
     std::cerr << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
     std::cerr << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
     glViewport(0, 0, 800, 600);
     unsigned int shaderProgram = Shader::compileShaders("shaders/vertexShader.vs","shaders/fragmentShader.fs",
          std::vector<std::string>({"aPos", "aColor", "aTexCoord"}));

     int width, height, nrChannels;
	unsigned char *data = stbi_load("assets/texture.jpg", &width, &height, &nrChannels, 0); 
	if(!data) std::cerr<<"Failed to load texture!\n";
	std::cout<<width<< " x "<<height<<" x "<<nrChannels<<"\n";
	unsigned int texture;
	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture);  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

     const int TRIANGLE_COUNT = 2; // bad code
     float vertices2[3*8*TRIANGLE_COUNT] = {
     // posX   posY   posZ   R     G     B     S     T
     // First triangle
     -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom-left
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom-right
          0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right

     // Second triangle
     -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom-left
          0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top-right
     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top-left
     };

     int triangle_count = 0;
     float* vertices = STLLoader::loadModel("models/king.stl",triangle_count);
     //memcpy(vertices, vertices2, 24*4);
     #define debug(x) std::cerr<<#x<<" = "<<x<<"\n"
     debug(vertices[0]);
     debug(vertices[1]);
     debug(vertices[2]);
     debug(vertices[8]);
     debug(vertices[9]);
     debug(vertices[10]);
     debug(vertices[8+8]);
     debug(vertices[8+9]);
     debug(vertices[8+10]);
     debug(vertices[32+10]);



     unsigned int VAO;
	glGenVertexArrays(1, &VAO);  
	glBindVertexArray(VAO);
	unsigned int VBO; //Vertex Buffer Object
	glGenBuffers(1, &VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
     glBufferData(GL_ARRAY_BUFFER,triangle_count*3*8*4, vertices, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Then we can make a call to the glBufferData function 
	//that copies the previously defined vertex data into the buffer's memory
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  //we need to tell OpenGL how to interpret vertex data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);  

     



     glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
     glDisable(GL_DEPTH_TEST);
     std::cerr<<triangle_count<<"\n";
     sf::Clock clock;
     float timet;
     glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
     while(window.isOpen()){
          static float last_time = 0.0;
          static float new_time = 0.0;
          static int iter = 0; iter++;
          if(iter % 256 == 0){
               new_time = clock.getElapsedTime().asMilliseconds();
               std::cerr<<(int)(256000.0/(new_time-last_time))<<" FPS\n";
               last_time = new_time;
          }
          
          //std::cerr<<clock.getElapsedTime().asMilliseconds()<<"\n";
          sf::Event event;
          while(window.pollEvent(event)){
               if(event.type == sf::Event::Closed){
                    window.close();
               }else if(event.type == sf::Event::Resized){
                    glViewport(0,0,event.size.width, event.size.height);
               }
          }

          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, triangle_count*3*8*4, vertices);


          const int width = 800; const int height = 600;
          glm::mat4 proj = glm::perspective(glm::radians((float)(175.0f)), (float)width/(float)height, 0.01f, 100.0f);
          glm::mat4 model,view;
          model = view = glm::mat4(1.0f);
          view = glm::rotate(view,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
          view = glm::rotate(view,glm::radians(timet*10.0f),glm::vec3(0.0f,0.0f,1.0f));

          int modelLoc = glGetUniformLocation(shaderProgram, "model");
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
          int viewLoc = glGetUniformLocation(shaderProgram, "view");
          glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
          int projLoc = glGetUniformLocation(shaderProgram, "projection");
          glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
          timet = ((float)clock.getElapsedTime().asMilliseconds())*0.001;
          int timeLoc = glGetUniformLocation(shaderProgram, "time");
          glUniform1f(timeLoc, timet);
          glm::vec4 punkt(vertices[0],vertices[1],vertices[2],1.0f);
          punkt = proj * view  * model *  punkt;
          //std::cerr<<punkt.x<<","<<punkt.y<<","<<punkt.z<<","<<punkt.w<<"\n";


		glDrawArraysInstanced(GL_TRIANGLES, 0, triangle_count*3*8/*amount of vertices*/,1 /*count of instances*/);

          window.display();
     }



     return 0;
}

