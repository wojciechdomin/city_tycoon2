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
     #define debug(x) std::cerr<<#x<<" = "<<x<<"\n"
     int triangle_count = 0;
     //float* vertices = STLLoader::loadModel("models/king.stl",triangle_count);
     debug("!");
     float* vertices = new float[480000];
     for(int i = 0; i < 100; i++){
     	for(int j = 0; j < 100; j++){

     		memcpy(vertices + ((i + j*100)*48), vertices2, 96*2);
     		for(int k = ((i + j*100)*48); k < ((i + j*100)*48) + 48; k+=8){
     			vertices[k] += (float)j;
     			vertices[k+1] += (float)i;
     			vertices[k+2] += 0.0f;
     		}
     	}
     }
     debug("?");
     triangle_count = 20000;
     //memcpy(vertices, vertices2, 24*4);
     
   



     unsigned int VAO[3];
	glGenVertexArrays(3, VAO);  
	glBindVertexArray(VAO[0]);
	unsigned int VBO[3]; //Vertex Buffer Object
	glGenBuffers(3, VBO);  
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);  
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
//second vao - vbo pair
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	int triangle_count3;
	float* vertices3 = STLLoader::loadModel("models/king.stl", triangle_count3);
	debug(triangle_count3);
	glBufferData(GL_ARRAY_BUFFER, triangle_count3 * 3*8*4, vertices3, GL_STATIC_DRAW);

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
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
     glm::vec3 camera_position(0.0f,0.0f,0.0f);
     debug("Starting main loop");
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
               }else if(event.type == sf::Event::KeyPressed){
               	float step = 0.06f;
               		switch(event.key.code){
               			case int('A'-'A'):
               					camera_position.x += step; 
               				break;
               			case int('D'-'A'):
               					camera_position.x -= step; 
               				break;
               			case int('W'-'A'):
               					camera_position.y += step; 
               				break;
               			case int('S'-'A'):
               					camera_position.y -= step; 
               				break;
               			case int('Q'-'A'):
               					camera_position.z += step; 
               				break;
               			case int('E'-'A'):
               					camera_position.z -= step; 
               				break;

               			default:
               			break;
               		}
               }
          }

          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO[0]);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, triangle_count*3*8*4, vertices);


          const int width = 800; const int height = 600;
          glm::mat4 proj = glm::perspective(glm::radians((float)(45.0f)), (float)width/(float)height, 0.1f, 10.0f);
          glm::mat4 model,view;
          model = view = glm::mat4(1.0f);
          model = glm::rotate(model,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f));
          model = glm::rotate(model,glm::radians(timet*0.1f),glm::vec3(0.0f,0.0f,1.0f));
          view = glm::translate(view, camera_position);

          int modelLoc = glGetUniformLocation(shaderProgram, "model");
          glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
          int viewLoc = glGetUniformLocation(shaderProgram, "view");
          glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
          int projLoc = glGetUniformLocation(shaderProgram, "projection");
          glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
          timet = ((float)clock.getElapsedTime().asMilliseconds())*0.001;
          int timeLoc = glGetUniformLocation(shaderProgram, "time");
          glUniform1f(timeLoc, timet);
          
     	  
          //std::cerr<<punkt.x<<","<<punkt.y<<","<<punkt.z<<","<<punkt.w<<"\n";


		//glDrawArraysInstanced(GL_TRIANGLES, 0, triangle_count*3*8/*amount of floats*/,1 /*count of instances*/);
          glDrawArrays(GL_TRIANGLES, 0, triangle_count*3*8);



          glBindVertexArray(VAO[1]);
          //glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
		  glDrawArrays(GL_TRIANGLES, 0, triangle_count3*3*8);          
          window.display();
          


     }



     return 0;
}

