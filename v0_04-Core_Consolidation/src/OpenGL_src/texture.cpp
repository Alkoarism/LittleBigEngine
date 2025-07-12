#include "OpenGL/texture.h"

Texture::Texture(GLenum target, GLenum format) : m_target(target), m_format(format) {
	glGenTextures(1, &m_textureID);
	m_updated = false;

	SetPar(GL_TEXTURE_WRAP_S, GL_REPEAT);
	SetPar(GL_TEXTURE_WRAP_T, GL_REPEAT);
	SetPar(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	SetPar(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	Bind();
}

Texture::Texture(Texture&& other) noexcept
	: m_textureID(other.m_textureID), m_target(other.m_target), m_format(other.m_format) {
	other.m_textureID = 0;
}

Texture::~Texture() {
	Release();
}

Texture& Texture::operator=(Texture&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_textureID, other.m_textureID);
		std::swap(this->m_target, other.m_target);
		std::swap(this->m_format, other.m_format);
	}
	return *this;
}

void Texture::Load(const void* texture, const int& width,const int& height) {
	glTexImage2D(this->m_target, 0, this->m_format, width, height,
		0, this->m_format, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(this->m_target);
	
	if (texture == nullptr)
		std::cout << "ERROR::TEXTURE::UNDEFINED_TEXT_PTR" << std::endl;
}

void Texture::SetPar(const GLenum& pName, const GLenum& param) {
	m_layout[pName] = param;
	m_updated = false;
}

void Texture::Bind() const {
	glBindTexture(this->m_target, m_textureID);
	Update();
}

void Texture::Unbind() const {
	glBindTexture(this->m_target, 0);
}