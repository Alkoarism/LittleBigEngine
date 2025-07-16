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

    if (m_textures.find(name) != m_textures.end()) return *m_textures[name];

    ImgLoader img(file);
    
    if (img.GetLog() != nullptr){
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: "
            << name << "\n" 
            << "->ERROR_CODE: " << img.GetLog() << std::endl;
        assert(m_textures.find(LBE_ERROR_TEXTURE_NAME) != m_textures.end());
        return *m_textures[LBE_ERROR_TEXTURE_NAME];
    }

    GLenum format;
    switch(img.GetChannels()){
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = GL_RGB; break;
    }
    m_textures.emplace(name, new Texture(GL_TEXTURE_2D, format));
    Texture& texture = *m_textures[name];
    texture.Bind();
    texture.Load(img.GetData(), img.GetWidth(), img.GetHeight());
    
    return texture;
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
        return *m_textures.at(name);
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
std::map<std::string, std::unique_ptr<Texture>> Things::m_textures;
std::map<std::string, Model> Things::m_models;