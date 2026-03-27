#include <shader.h>

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

	const unsigned int
		vertex = glCreateShader(GL_VERTEX_SHADER),
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

	int success; char infoLog[512];
	glShaderSource(vertex, 1, &vc_ptr, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		std::cerr << "Vertex shader compilation error!\n";
		return;
	}

	glShaderSource(vertex, 1, &vc_ptr, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		std::cerr << "Vertex shader compilation error!\n";
		return;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		std::cerr << "Program linking error!\n";
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
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, const int &value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, const float &value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
