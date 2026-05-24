#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

class IndexBuffer {
public:
	IndexBuffer(const GLuint*,const GLuint);

	~IndexBuffer() { Release(); }

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;

	inline GLuint GetCount() const { return m_indexCount; }

private:
	GLuint m_indexBufferID = 0;
	GLuint m_indexCount = 0;

	void Release() noexcept {
		glDeleteBuffers(1, &m_indexBufferID);
		m_indexBufferID = 0;
	}
};

#endif