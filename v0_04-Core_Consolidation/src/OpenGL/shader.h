#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

class Shader {
public:
	Shader() { }
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	void Use() const;
	void Compile(const char*, const char*, const char* geometrySource = nullptr);

	template <typename T>
	bool SetUniform(const std::string&, const T&) const;
	template <typename T>
	bool GetUniform(const std::string&, T&) const;

private:
	GLuint m_shaderID;

	void Release() noexcept {
		glDeleteProgram(m_shaderID);
		m_shaderID = 0;
	}

	//utility uniform functions- template classes to avoid nondefined types to compile
	template <typename T>
	void SetGLUniform(const std::string&, const T&) const {
		throw std::invalid_argument("ERROR::SHADER::INVALID_UNIFORM_TYPE");
	}
	void SetGLUniform(const std::string&, const bool&) const;
	void SetGLUniform(const std::string&, const int&) const;
	void SetGLUniform(const std::string&, const float&) const;
	void SetGLUniform(const std::string&, const glm::vec3) const;
	void SetGLUniform(const std::string&, const glm::vec4) const;
	void SetGLUniform(const std::string&, const glm::mat4&) const;

	template <typename T>
	void GetGLUniform(const std::string&, const T&) const {
		throw std::invalid_argument("ERROR::SHADER::UNIFORM_TYPE_NOT_FOUND");
	}
	void GetGLUniform(const std::string&, int&) const;
	void GetGLUniform(const std::string&, float&) const;

	void CheckCompileErrors(unsigned int shader, std::string type);
};

template <typename T>
bool Shader::SetUniform(const std::string& name, const T& value) const {
	this->Use();
	try {
		SetGLUniform(name, value);
		return true;
	}
	catch (std::invalid_argument err) {
		std::cout << "COULD_NOT_DEFINE_UNIFORM: \"" << name << "\"\n"
			<< err.what() << std::endl;
		return false;
	}
}

template <typename T>
bool Shader::GetUniform(const std::string& name, T& value) const {
	this->Use();
	try {
		GetGLUniform(name, value);
		return true;
	}
	catch (std::invalid_argument err) {
		std::cout << "COULD_NOT_FIND_UNIFORM: \"" << name << "\"\n"
			<< err.what() << std::endl;
		return false;
	}
}


#endif