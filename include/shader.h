#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string> 
#include <glm/glm.hpp>

class Shader{
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const std::string &name, const bool& value) const;
		void setInt(const std::string &name, const int& value) const;
		void setFloat(const std::string &name, const float& value) const;
        void setVec3(const std::string &name, 
                    const glm::vec3 &value);
        void setMat4(const std::string &name,
                        const glm::mat4 &matrix);
};

#endif
