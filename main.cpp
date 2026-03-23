#include <iostream> //Debug printing and e.t.c.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream> //For loading shaders from the files
#include <iterator>

void framebuffer_size_callback(GLFWwindow* , int width, int height);
void processInput(GLFWwindow* window);

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
		std::cerr << "Failed to initialize GLAD!\n";
		return -1;
	}

	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//-----------DRAWING----------------------------------
	
	float verticies[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, -0.0f,
		0.0f, 0.5f, 0.0f
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

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
		

	while(!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
