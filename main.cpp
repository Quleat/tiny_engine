#include <iostream> //Debug printing and e.t.c.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream> //For loading shaders from the files
#include <iterator>

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

	//--------------FRAGMENT SHADER 2--------
	const std::string fr2_sh_str(read_shader("fragment_shader2.glsl"));
	const char* const fr2_str_ptr = fr2_sh_str.c_str();
	const unsigned int fr2_sh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fr2_sh, 1, &fr2_str_ptr, NULL);
	glCompileShader(fr2_sh);
	glGetShaderiv(fr2_sh, GL_COMPILE_STATUS, &success);

	if(!success){
	std::cerr << "ERROR COMPILING FRAGMENT SHADER!!!\n";
	return -5;
	}
	//PROGRAM 2
	
	const unsigned int shaderProgram2 =
		glCreateProgram();
	glAttachShader(shaderProgram2, vertex_shader);
	glAttachShader(shaderProgram2, fr2_sh);
	glLinkProgram(shaderProgram2);
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if(!success){
	std::cerr << "Program LINK ERROR!\n";
	return -6;
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
		-0.75f, -0.5f, 0.0f,
    -0.25f, -0.5f, 0.0f,
    -0.5f, 0.25f, 0.0f,

    0.25f, -0.5f, 0.0f,
    0.75f, -0.5f, 0.0f,
    0.5f, 0.25f, 0.0f
	};
  /*unsigned int indicies[] = {
    0, 1, 3,
    1, 2, 3
  };*/

  //VAO1
	unsigned int VBO[2], VAO[2];
	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);

	glGenBuffers(2, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies) / 2, verticies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  //VAO2
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies) / 2, &verticies[9], GL_STATIC_DRAW); //why not verticies + (9 * sizeof(float))
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  /*glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);*/

	while(!glfwWindowShouldClose(window)){
		processInput(window);

    glPolygonMode(GL_FRONT_AND_BACK, /*GL_FILL*/GL_FILL);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	  glUseProgram(shaderProgram);
	  glBindVertexArray(VAO[0]);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	  //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
