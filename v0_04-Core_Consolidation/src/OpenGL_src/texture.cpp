#include "OpenGL/texture.h"

#include <utility>
#include <iostream>

Texture::Texture(const GLenum& target, const GLenum& format) : m_target(target), m_format(format) {
	glGenTextures(1, &m_textureID);
	m_instanceBitUnpackingSize = 4;
}

Texture::Texture(Texture&& other) noexcept
	: m_textureID(other.m_textureID), m_target(other.m_target), m_format(other.m_format),
	m_instanceBitUnpackingSize(other.m_instanceBitUnpackingSize) {
	other.m_textureID = 0;
	other.m_target = 0;
	other.m_format = 0;
	other.m_instanceBitUnpackingSize = 4;
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
		std::swap(this->m_instanceBitUnpackingSize, other.m_instanceBitUnpackingSize);
	}
	return *this;
}

void Texture::Load(const void* texture, const int& width,const int& height) {
	if (m_instanceBitUnpackingSize != m_bitUnpackingSize){
		m_bitUnpackingSize = m_instanceBitUnpackingSize;
		glPixelStorei(GL_UNPACK_ALIGNMENT, m_bitUnpackingSize);
	}

	glTexImage2D(this->m_target, 0, this->m_format, width, height,
		0, this->m_format, GL_UNSIGNED_BYTE, texture);
	glGenerateMipmap(this->m_target);
	
	if (texture == nullptr)
		std::cout << "ERROR::TEXTURE::UNDEFINED_TEXT_PTR" << std::endl;
}

void Texture::SetPar(const GLenum& pName, const GLenum& param) {
	m_layout[pName] = param;
	glTexParameteri(m_target, pName, param);
}

void Texture::SetBitUnpackingSize(const GLint& size){
	if (size == 1 || size == 2 || size == 4 || size == 8){
		m_instanceBitUnpackingSize = size;
	} else {
		std::cout << "ERROR::TEXTURE::BIT_UNPACKING_SIZE::INVALID_SIZE: " + size << std::endl;
	}
}

void Texture::Bind() const {
	glBindTexture(this->m_target, m_textureID);
}

void Texture::Unbind() const {
	glBindTexture(this->m_target, 0);
}

GLint Texture::m_bitUnpackingSize = 4;