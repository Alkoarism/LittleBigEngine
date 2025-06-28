#include "World/things.h"

Shader& Things::LoadShader
	(const std::string name, const char* vertPath, const char* fragPath) {
	
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_TO_LOAD_FILE" << std::endl;
    }

    m_Shaders.emplace(name, Shader());
    m_Shaders[name].Compile(vertexCode.c_str(), fragmentCode.c_str());
    return m_Shaders[name];
}

Texture& Things::LoadTexture
    (std::string name, const char* file, bool flipImage) {

    m_Textures.emplace(name, Texture());
    ImgLoader img(file);
    
    if (img.GetLog() == nullptr) {
        switch(img.GetChannels()){
            case 3: m_Textures.at(name).format = GL_RGB; break;
            case 4: m_Textures.at(name).format = GL_RGBA; break;
            default: m_Textures.at(name).format = GL_RGB; break;
        }
        m_Textures.at(name).Load(img.GetData(), img.GetWidth(), img.GetHeight());
    }
    else {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: "
            << name << "\n" 
            << "->ERROR_CODE: " << img.GetLog() << std::endl;
    }
    return m_Textures.at(name);
}

Texture& Things::LoadTexture(std::string name) {
    m_Textures.emplace(name, Texture());
    return m_Textures.at(name);
}

Shader& Things::GetShader(const std::string name) {
    try {
        return m_Shaders.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::SHADER::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

Texture& Things::GetTexture(const std::string name) {
    try {
        return m_Textures.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::TEXTURE::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

std::map<std::string, Shader> Things::m_Shaders;
std::map<std::string, Texture> Things::m_Textures;