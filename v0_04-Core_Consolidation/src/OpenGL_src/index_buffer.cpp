#include "OpenGL/index_buffer.h"

#include <utility>

IndexBuffer::IndexBuffer(const GLuint* data,const GLuint count) : m_indexCount(count) {
	glGenBuffers(1, &m_indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_indexBufferID(other.m_indexBufferID) {
	other.m_indexBufferID = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept{
	if (this != &other) {
		Release();
		std::swap(m_indexBufferID, other.m_indexBufferID);
	}
	return *this;
}

void IndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
}

void IndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}