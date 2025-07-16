#ifndef THINGS_H
#define THINGS_H

#include "headers.h"
#include "engine_config.h"

#include "OpenGL/vertex_array.h"
#include "OpenGL/index_buffer.h"
#include "OpenGL/shader.h"
#include "OpenGL/texture.h"

#include "World/entity.h"
#include "World/model.h"

#include "Modules/imgLoader/imgLoader.h"

//Stores all the data directly managed by the Engine on a currently running program
class Things{
public:
	static Shader& LoadShader
	(const std::string& name, const char* vertPath, const char* fragPath);
	static Texture& LoadTexture
	(const std::string& name, const char* file, bool flipImage);
	static Model& LoadModel(const std::string& name);

    static Shader& GetShader(const std::string& name);
	static Texture& GetTexture(const std::string& name);
	static Model& GetModel(const std::string& name);

private:
	static std::map<std::string, Shader> m_shaders;
	static std::map<std::string, std::unique_ptr<Texture>> m_textures;
    static std::map<std::string, Model> m_models;
};

#endif