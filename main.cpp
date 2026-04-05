#include <iostream> //Debug printing and e.t.c.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream> //For loading shaders from the files
#include <iterator>
#include <cmath>

#include <shader.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* , int width, int height);
void processInput(GLFWwindow* window);


int main(int argc, char *argv[]){
	//---------------CREATING A WINDOW-----------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if(window == NULL){
		std::cerr << "Failed to crate window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//------------GLAD------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cerr << "Failed to initialize GLAD!!\n";
		return -1;
	}

	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	//-----------SHADERS----------------------------------
	Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
	//-----------DRAWING----------------------------------
	float vertices[] = {
	    // positions          // colors           // texture coords
	     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indicies[] = {
		0, 1, 2, 3, 0, 2
	};

	//VAO1
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	std::cerr << sizeof(float) << '\n';

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); //COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glBindVertexArray(0);
	shader.use();

	//----------LOADING TEXTURES----------
	unsigned int texture; 
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	//Image 1
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if(!data){
		std::cerr << "Failed to load the texture!\n";
		return -10;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	//Image 2
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	unsigned char *data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if(!data){
		std::cerr << "Failed to load the texture!\n";
		return -10;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data2);

	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shader.ID, "texture2"), 1);

	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO);

		//First box
		glm::mat4 tn = glm::mat4(1.0f);
		tn = glm::translate(tn, glm::vec3(0.5f, -0.5f, 0.0f));
		tn = glm::rotate(tn, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		tn = glm::scale(tn, glm::vec3(0.5f, 0.5f, 0.5f));
		const int transform_loc = glGetUniformLocation(shader.ID, "transform");
		if(transform_loc == -1){
			std::cerr << "Matrix not found!\n";
		}
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(tn));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Second box
		glm::mat4 new_tn = glm::mat4(1.0f);
		new_tn = glm::translate(new_tn, glm::vec3(-0.5f, 0.5f, 0.0f));
		const float coef = (std::sin((float)glfwGetTime()) + 1) / 2;
		new_tn = glm::scale(new_tn, glm::vec3(coef, coef, 0.0f));
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(new_tn));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void framebuffer_size_callback(GLFWwindow* , int width, int height){
	glViewport(0,0,width,height);
}
void processInput(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
