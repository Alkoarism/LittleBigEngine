#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

class VertexBuffer {
public:
	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Update(const void* data, unsigned int size, unsigned int offset);
	void Bind() const;
	void Unbind() const;

private:
	GLuint m_vertexBufferID = 0;
	void Release() noexcept {
		glDeleteBuffers(1, &m_vertexBufferID);
		m_vertexBufferID = 0;
	}
};

#endif