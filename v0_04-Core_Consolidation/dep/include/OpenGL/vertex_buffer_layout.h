#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "headers.h"

//more types can be added to this code as the need arises.

struct VertexAttribute {
	VertexAttribute(GLuint t, GLuint c, GLboolean b) {
		type = t;
		count = c;
		normalized = b;
	}

	GLuint type;
	GLuint count;
	GLboolean normalized;

	static unsigned int GetSizeOfType(GLuint type){
		switch (type) {
		case (GL_FLOAT): return sizeof(float);
		}
		assert(false);
		return 0;
	}

};

class VertexBufferLayout {
public:
	VertexBufferLayout()
		: m_Stride(0){ }

	template<typename T>
	void Push(const int count) {
		static_assert(sizeof(T) == 0, "ERROR::VERTEXBUFFERLAYOUT::Unsupported_type");
	}

	inline const std::vector<VertexAttribute> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexAttribute> m_Elements;
	unsigned int m_Stride;
};

template<>
inline void VertexBufferLayout::Push<float>(const int count) {
	m_Elements.push_back(VertexAttribute(GL_FLOAT, count, GL_FALSE));
	m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<int>(const int count) {
	m_Elements.push_back(VertexAttribute(GL_INT, count, GL_FALSE));
	m_Stride += count * VertexAttribute::GetSizeOfType(GL_INT);
}

#endif