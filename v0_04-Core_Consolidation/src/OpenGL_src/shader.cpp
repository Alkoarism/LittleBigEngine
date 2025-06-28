#include "OpenGL/shader.h"

Shader::~Shader() {
    Release();
}

Shader::Shader(Shader&& other) noexcept 
    :   m_shaderID(other.m_shaderID) {
    other.m_shaderID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        Release();
        std::swap(m_shaderID, other.m_shaderID);
    }
    return *this;
}

void Shader::Use() const{
    glUseProgram(m_shaderID);
}

void Shader::Compile(const char* vShaderCode, const char* fShaderCode) {
    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    if (m_shaderID != 0){
        Release();
    }
    m_shaderID = glCreateProgram();
    glAttachShader(m_shaderID, vertex);
    glAttachShader(m_shaderID, fragment);
    glLinkProgram(m_shaderID);
    CheckCompileErrors(m_shaderID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::SetGLUniform(const std::string& name, const bool& value) const {
    glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value);
}

void Shader::SetGLUniform(const std::string& name, const int& value) const {
    glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
}

void Shader::SetGLUniform(const std::string& name, const float& value) const {
    glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
}

void Shader::SetGLUniform(const std::string& name, const glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(m_shaderID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetGLUniform(const std::string& name, const glm::vec4 value) const {
    glUniform4f(glGetUniformLocation(m_shaderID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetGLUniform(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv
        (glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::GetGLUniform(const std::string& name, int& value) const {
    glGetUniformiv(m_shaderID, glGetUniformLocation(m_shaderID, name.c_str()), &value);
}

void Shader::GetGLUniform(const std::string& name, float& value) const {
    glGetUniformfv(m_shaderID, glGetUniformLocation(m_shaderID, name.c_str()), &value);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " 
                << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}