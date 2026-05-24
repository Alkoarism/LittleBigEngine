#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "OpenGL/vertex_buffer.h"
#include "OpenGL/vertex_buffer_layout.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void AddBuffer(const VertexBuffer&, const VertexBufferLayout&);

	void Bind() const;
	void Unbind() const;

private:
	GLuint m_vertexArrayID = 0;
	void Release() noexcept {
		glDeleteVertexArrays(1, &m_vertexArrayID);
		m_vertexArrayID = 0;
	}
};

#endif