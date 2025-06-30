#include "World/things.h"

Shader& Things::LoadShader
	(const std::string& name, const char* vertPath, const char* fragPath) {
	
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

    m_shaders.emplace(name, Shader());
    m_shaders[name].Compile(vertexCode.c_str(), fragmentCode.c_str());
    return m_shaders[name];
}

Texture& Things::LoadTexture
    (const std::string& name, const char* file, bool flipImage) {

    m_textures.emplace(name, Texture());
    Texture& texture = m_textures[name];
    
    ImgLoader img(file);
    
    if (img.GetLog() == nullptr) {
        switch(img.GetChannels()){
            case 3: texture.format = GL_RGB; break;
            case 4: texture.format = GL_RGBA; break;
            default: texture.format = GL_RGB; break;
        }
        texture.Load(img.GetData(), img.GetWidth(), img.GetHeight());
    }
    else {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: "
            << name << "\n" 
            << "->ERROR_CODE: " << img.GetLog() << std::endl;
    }
    return texture;
}

Texture& Things::LoadTexture(const std::string& name) {
    return m_textures[name];
}

Model& Things::LoadModel(const std::string& name) {
    return m_models[name];
}

Shader& Things::GetShader(const std::string& name) {
    try {
        return m_shaders.at(name);
    }
    catch (const std::out_of_range& e) {
        std::cout << "ERROR::THINGS::SHADER::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

Texture& Things::GetTexture(const std::string& name) {
    try {
        return m_textures.at(name);
    }
    catch (const std::out_of_range& e) {
        std::cout << "ERROR::THINGS::TEXTURE::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

Model& Things::GetModel(const std::string& name) {
    try {
        return m_models.at(name);
    }
    catch (const std::out_of_range& e) {
        std::cout << "ERROR::THINGS::MODEL::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

std::map<std::string, Shader> Things::m_shaders;
std::map<std::string, Texture> Things::m_textures;
std::map<std::string, Model> Things::m_models;