#include "logger.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include "model.h"

#include "shader.h"
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* , int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void recalculate_mouse_rotation();

glm::vec3 cameraPos 	= glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront 	= glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp 	= glm::vec3(0.0f, 1.0f, 0.0f);

float currentFrame = 0;

//Default mouse positions
double lastX = 400, lastY = 300;
double yaw = -90, pitch = 0;

int main(){
	//---------------CREATING A WINDOW-----------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "tiny_engine", NULL, NULL);
    if (window == NULL){
        print_error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

	//------------GLAD------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		print_error("Failed to initialize GLAD!!");
		return -1;
	}

	glViewport(0,0,1024,720);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //-----------OPENGL---------------------------------

    //Vertecies


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

	//VAO1
	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

    //Light cube VAO
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
	glBindVertexArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glBindVertexArray(0);

    //Model backpack_model("/home/quleat/Projects/tiny_engine/models/backpack/backpack.obj");

    //compiling shaders
	Shader light_shader("shaders/vertex_shader.glsl", "shaders/light_fragment_shader.glsl");
    Shader simple_shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    //Light cube position
    glm::vec3 light_pos(0.0f, 0.0f, 0.0f);
    glm::mat4 light_model = glm::mat4(1.0f);
    light_model = glm::translate(light_model, light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));

    //Setting up the projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    light_shader.use();
    light_shader.setMat4("model", light_model);
    light_shader.setMat4("projection", projection);

    //The second cube
    glm::vec3 cube_pos(2.0f, 0.0f, 0.0f);
    glm::mat4 cube_model = glm::mat4(1.0f);
    cube_model = glm::translate(cube_model, cube_pos);
    
    simple_shader.use();
    simple_shader.setVec3("material.ambient", {1.0f, 0.5f, 0.31f});
    simple_shader.setVec3("material.diffuse", {1.0f, 0.5f, 0.31f});
    simple_shader.setVec3("material.specular", {0.5f, 0.5f, 0.5f});
    simple_shader.setFloat("material.shininess", 32.0f);
    simple_shader.setVec3("lightColor", {1.0, 1.0f, 1.0f});
    simple_shader.setMat4("model", cube_model);
    simple_shader.setMat4("projection", projection);

	glEnable(GL_DEPTH_TEST);
	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//View matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
	    glBindVertexArray(lightVAO);
        light_shader.use();
        light_shader.setMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

	    glBindVertexArray(cubeVAO);
        simple_shader.use();
        //Moving the cube
        cube_model = glm::mat4(1.0f);
        cube_pos.z = std::sin(glfwGetTime()) * 2;
        cube_pos.x = std::sin(glfwGetTime() + 2) * 2;
        cube_model = glm::translate(cube_model, cube_pos);
        simple_shader.setMat4("model", cube_model);
        simple_shader.setMat4("view", view);
        simple_shader.setVec3("lightPos", light_pos);
        simple_shader.setVec3("viewPos", cameraPos);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //backpack_model.Draw(shader);
		//Boxes
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

    //Keyboard movement
    const float speed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= speed * glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));

    //Keyboard view rotation
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        pitch += 0.5;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        pitch -= 0.5;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        yaw += 0.5;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        yaw -= 0.5;
    recalculate_mouse_rotation();
}

bool first_move=true;

void mouse_callback(GLFWwindow*, double xpos, double ypos){
  ypos *= -1;

  if(first_move){
    lastX = xpos; 
    lastY = ypos;
    first_move = false;
  }
  const double sensetivity = 0.1;
  const double xoffset = (xpos - lastX) * sensetivity;
  const double yoffset = (ypos - lastY) * sensetivity;

  lastX = xpos;
  lastY = ypos;

  yaw += xoffset;
  pitch += yoffset;

  recalculate_mouse_rotation();
}

void recalculate_mouse_rotation(){
  if(pitch > 89.0)
    pitch = 89.0;
  if(pitch < -89.0)
    pitch = -89.0;

  glm::vec3 direction;
  direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  direction.y = std::sin(glm::radians(pitch));
  direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}
