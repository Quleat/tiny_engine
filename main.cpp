#include <iostream> //Debug printing and e.t.c.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream> //For loading shaders from the files
#include <iterator>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* , int width, int height);
void processInput(GLFWwindow* window);

std::string read_shader(const std::string &path){
	std::ifstream stream(path);
	if(!stream){
		std::cerr << "Error opening file: " << path << '\n';
		return "";
	}
	std::istreambuf_iterator<char> beg_fs(stream);
	std::istreambuf_iterator<char> end_fs;

	const std::string result(
			beg_fs,
			end_fs);
	return result;
}

int main(int, char *[]){
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



	//--------------VERTEX SHADER---------------
  	//Reading the vertex shader
	std::fstream file_vertex_shader("vertex_shader.glsl");
	if(!file_vertex_shader){
		std::cerr << "Couldn't open the vertex shader file!\n";
		return 1;
	}
	std::istreambuf_iterator<char> begin(file_vertex_shader);
	std::istreambuf_iterator<char> end;
	const std::string vertex_shader_source(begin, end);

	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* const vsh_ptr = vertex_shader_source.c_str(); //glShader Needs char**
	glShaderSource(vertex_shader, 1, &vsh_ptr, NULL);
	glCompileShader(vertex_shader);

	int success; 
	char infoLog[256];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cerr << "Vertex shader compilation error: " << '\n'
			<< infoLog << '\n';
	}
	else
		std::cout << "Vertex shader: OK\n";


	//--------------FRAGMENT SHADER---------------
	const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	std::ifstream file_fs("fragment_shader.glsl");
	std::istreambuf_iterator<char> beg_fs(file_fs);
	std::istreambuf_iterator<char> end_fs;
	const std::string fragment_shader_str(
			beg_fs, end_fs);

	const char* const fs_str_ptr = fragment_shader_str.c_str();
	glShaderSource(fragmentShader, 1, &fs_str_ptr, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if(!success){
		std::cout << "FRAGMENT SHADER ERROR!!!\n";
		std::cout << fragment_shader_str << '\n';
		return -5;
	}

	const unsigned int shaderProgram =
		glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		std::cerr << "PROGRAM LINK ERROR!\n";
		return -6;
	}
	//glUseProgram(shaderProgram);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragmentShader);

	//-----------DRAWING----------------------------------
	
	float verticies[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};
  unsigned int indicies[] = {
    0, 1, 2
  };

  //VAO1
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); //Coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  glBindVertexArray(0);


	glUseProgram(shaderProgram);
	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

    //const float timeValue = glfwGetTime();
    //const float greenValue = ((std::sin(timeValue)) / 2.0f) + 0.5f;

    //const int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	  glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	  //glDrawArrays(GL_TRIANGLES, 0, 3);

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
