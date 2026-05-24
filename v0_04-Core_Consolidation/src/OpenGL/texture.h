#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <map>

class Texture {
public:
	Texture(const GLenum& target, const GLenum& format);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void Load(const void*, const int&, const int&);
	void SetPar(const GLenum& pName, const GLenum& param);
	//Only accepts one of the following values: 1, 2, 4 or 8; due to OpenGL specifications
	void SetBitUnpackingSize(const GLint& size);
	//Throws an error when run with invalid PNames due to not knowing standard GL values
	const GLuint GetPar(const GLenum& pName) const { return m_layout.at(pName); }
	
	void Bind() const;
	void Unbind() const;

private:
	GLenum m_target;
	GLenum m_format;
	GLuint m_textureID;
	std::map<GLenum, GLenum> m_layout;

	void Release() noexcept {
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}

	//Controls bit unpacking alignment across textures avoiding constant OpenGL state changes
	//by tracking global state via m_bitUnpackingSize (crucial for font loading)
	GLint m_instanceBitUnpackingSize;
	static GLint m_bitUnpackingSize;
};

#endif