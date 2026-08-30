#include "shader.h"
#include <iostream>
#include <fstream>
#include "logger.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	const std::string vertex_code = read_shader(vertexPath);
	const std::string fragment_code = read_shader(fragmentPath);
	
	const char* vc_ptr = vertex_code.c_str();
	const char* fc_ptr = fragment_code.c_str();
    std::cout << fc_ptr << std::endl;

	const unsigned int
		vertex = glCreateShader(GL_VERTEX_SHADER),
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

	int success; char infoLog[512];
	glShaderSource(vertex, 1, &vc_ptr, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		std::cerr << "Vertex shader compilation error!\n";
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cerr << "Message: " << infoLog << "\n";
		return;
	}

	glShaderSource(fragment, 1, &fc_ptr, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		std::cerr << "Fragment shader compilation error!\n";
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cerr << "Message: " << infoLog << "\n";
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		std::cerr << "Program linking error!\n";
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cerr << "Error message: " << infoLog << "\n";
		return;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, const bool &value) const
{
	const int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1){
		std::cerr << "[ERROR]: Couldn't locate the uniform: " << name << "\n";
        return;
	}
	glUniform1i(location, (int)value);
}
void Shader::setInt(const std::string &name, const int &value) const
{
	const int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1){
		std::cerr << "[ERROR]: Couldn't locate the uniform: " << name << "\n";
        return;
	}
	glUniform1i(location, value);
}
void Shader::setFloat(const std::string &name, const float &value) const
{
	const int location = glGetUniformLocation(ID, name.c_str());
	if (location == -1){
		std::cerr << "[ERROR]: Couldn't locate the uniform: " << name << "\n";
        return;
	}
	glUniform1f(location, value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value)
{
    const int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1){
        print_error("Could not locate the uniform! \'" + 
                    name + "\'");
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
}   
    
void Shader::setMat4(const std::string &name,
    const glm::mat4 &matrix){
    const int location = glGetUniformLocation(ID, name.c_str());
    if(location == -1){
        print_error("Could not locate the uniform! \'" + 
                    name + "\'");
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
