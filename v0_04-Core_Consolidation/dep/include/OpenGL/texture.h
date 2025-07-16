#ifndef TEXTURE_H
#define TEXTURE_H

#include "headers.h"

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
};

#endif