#include "OpenGL/vertex_buffer.h"

#include <utility>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage) {
	glGenBuffers(1, &m_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer() {
	Release();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept 
	: m_vertexBufferID(other.m_vertexBufferID) {
	other.m_vertexBufferID = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	if (this != &other) {
		Release();
		std::swap(m_vertexBufferID, other.m_vertexBufferID);
	}
	return *this;
}

void VertexBuffer::Update(const void* data, unsigned int size, unsigned int offset) {
	this->Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}