#ifndef THINGS_H
#define THINGS_H

#include "headers.h"
#include "OpenGL/vertex_array.h"
#include "OpenGL/index_buffer.h"
#include "OpenGL/shader.h"
#include "OpenGL/texture.h"

#include "World/entity.h"
#include "World/mesh.h"

#include "Modules/imgLoader/imgLoader.h"

//Stores all the data directly managed by the Engine on a currently running program
class Things{
public:
	static Shader& LoadShader
	(const std::string name, const char* vertPath, const char* fragPath);
	static Texture& LoadTexture(std::string name);
	static Texture& LoadTexture
	(std::string name, const char* file, bool flipImage);
	static Mesh& LoadMesh(std::string name);

    static Shader& GetShader(const std::string name);
	static Texture& GetTexture(const std::string name);
	static Mesh& GetTMesh(const std::string name);

private:
	static std::map<std::string, Shader> m_Shaders;
	static std::map<std::string, Texture> m_Textures;
    static std::map<std::string, Mesh> m_Meshes;
};

#endif